#include"player.h"
#include"actor.h"
#include"stack.h"
#include"list.h"
#include"point.h"
#include"aquarium.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Actor and its member variables
//		Remembers and discovers the starting point.
Player::Player(Aquarium* aquarium, Point p, std::string name, char sprite)
	:Actor(aquarium, p, name, sprite),
	m_look(),
	m_discovered(),
	m_btQueue(),
	m_btStack(),
	m_toggleBackTracking(false) {

	// Discover the starting point
	m_discovered.push_front(p);	// this is the LINKED LIST
	m_look.push(p);	// this is the QUEUE
}

//  stuck() 
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
	return Actor::getState() == State::STUCK;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
	return Actor::getState() == State::FREEDOM;
}
//  toggleBackTrack( ... )
//		Turn on/off backtracking
void Player::toggleBackTrack(bool toggle) {
	m_toggleBackTracking = toggle;
}

//  getTargetPoint()
//		Get the point the player wants to look around next.
//      If m_look is empty then return an invalid point.
Point Player::getTargetPoint() const {
	if (m_look.empty()) return Point(-1, -1);
	return m_look.front();
}

// discovered
//		returns true if the item is in the list
bool Player::discovered(const Point &p) const {
	return (m_discovered.find(p) != -1);
}



// say()
//		What does the player say?
//		Already Implemented, nothing to do here
void Player::say() {

	// Freedom supercedes being eaten
	if (getState() == State::FREEDOM) {
		std::cout << getName() << ": WEEEEEEEEE!";
		return;
	} 
 
	// Being eaten supercedes being lost
	switch (getInteract()) {
	case Interact::ATTACK:
		std::cout << getName() << ": OUCH!";
		break;
	case Interact::GREET:
		break;
	case Interact::ALONE:
	default:
		switch (getState()) {
		case State::LOOKING:
			std::cout << getName() << ": Where is the exit?";
			break;
		case State::STUCK:
			std::cout << getName() << ": Oh no! I am Trapped!";
			break;
		case State::BACKTRACK:
			std::cout << getName() << ": Got to backtrack...";
			break;
		default:
			break;
		}

		break;
	}
}


//  update() 
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOKING). update also handles moving the 
//		player as well, if there is an open undiscovered cell to move to the  
//		player just moves there.  However, there will be cases in the next 
//		desired point to look around is not adjacent (more than one cell away), 
//		at this point the player must backtrack (State::BACKTRACK) to a point 
//		that is adjacent to the next point the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOK or 
//		BACKTRACK not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop. Your
//		Implementation should NOT have any loops that pertain to actual movement 
//		of the player.  
//
//		Backtracking is challenging, save it for the very very very last thing.
//		Make sure the STATE::LOOKING aspect compiles and works first.
void Player::update() {
	Point p = m_look.front();
	int x = p.x();
	int y = p.y();
	int discoverCt = 0;
	if (m_toggleBackTracking == true) {
		if (m_btQueue.empty() == true)
			setState(State::LOOKING);

		if (getState() == State::BACKTRACK) {

			if (getState() == State::BACKTRACK) {
				moveTo(m_btQueue.front());
				// recycles front pointer to back of queue
				//  and then removes it from the front
				m_btQueue.push(m_btQueue.front());
				m_btQueue.pop();
			}
			// sets State back to LOOKING
			if (m_btQueue.front() == currentPosition()) {
				setState(State::LOOKING);
				return;
			}
		}
		// code for discovering
		else if (getState() == State::LOOKING) {
			moveTo(m_look.front());
			m_look.pop();

			// #6-- if slot to the WEST is open & is undiscovered
			//Mark (curX -1, curY) as "discovered"
			//PUSH (curX -1, curY) on stack.
			p.set((x - 1), y);
			if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
				m_look.push(p);
				m_discovered.push_rear(p);
				discoverCt++;
			}
			p = currentPosition();
			// #7-- if slot to the EAST is open & is undiscovered
			//		Mark (curX +1, curY) as "discovered"
			//		PUSH (curX +1, curY) on stack.
			p.set((x + 1), y);
			if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
				m_look.push(p);
				m_discovered.push_rear(p);
				discoverCt++;
			}
			p = currentPosition();
			// #8-- if slot to the NORTH is open & is undiscovered
			//		Mark (curX, curY -1) as "discovered"
			//		PUSH (curX, curY -1) on stack.
			p.set(x, (y - 1));
			if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
				m_look.push(p);
				m_discovered.push_rear(p);
				discoverCt++;
			}
			p = currentPosition();
			// #9-- if slot to the SOUTH is open & is undiscovered
			//		Mark (curX, curY +1) as "discovered"
			//		PUSH (curX, curY +1) on stack.
			p.set(x, (y + 1));
			if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
				m_look.push(p);
				m_discovered.push_rear(p);
				discoverCt++;
			}
			p = currentPosition();

			// the number of times a new room/route is discovered
			// will affect how many times the current position is
			// pushed to the back of the m_btQueue.
			switch (discoverCt) {
			case 4:
				m_btQueue.push(currentPosition());
			case 3:
				m_btQueue.push(currentPosition());
			case 2:
			case 1:
				m_btQueue.push(currentPosition());
				m_btQueue.push(currentPosition());
				break;
			default:
				break;
			}
			// sets state to STUCK or FREEDOM or BACKTRACK
			if (currentPosition() == getAquarium()->getEndPoint())
				setState(State::FREEDOM);
			else if (m_look.empty() == true)
				setState(State::STUCK);
			else if (m_btQueue.front() != currentPosition()) {
				setState(State::BACKTRACK);
			}
		}
	}
	else {
		moveTo(m_look.front());
		
		
		m_look.pop();
		// #6-- if slot to the WEST is open & is undiscovered
		//		Mark (curX -1, curY) as "discovered"
		//		PUSH (curX -1, curY) on stack.^
		p.set((x - 1), y);
		if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
			m_look.push(p);
			m_discovered.push_rear(p);
		}
		p = currentPosition();
		// #7-- if slot to the EAST is open & is undiscovered
		//		Mark (curX +1, curY) as "discovered"
		//		PUSH (curX +1, curY) on stack.
		p.set((x + 1), y);
		if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
			m_look.push(p);
			m_discovered.push_rear(p);
		}
		p = currentPosition();
		// #8-- if slot to the NORTH is open & is undiscovered
		//		Mark (curX, curY -1) as "discovered"
		//		PUSH (curX, curY -1) on stack.
		p.set(x, (y - 1));
		if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
			m_look.push(p);
			m_discovered.push_rear(p);
		}
		p = currentPosition();
		// #9-- if slot to the SOUTH is open & is undiscovered
		//		Mark (curX, curY +1) as "discovered"
		//		PUSH (curX, curY +1) on stack.
		p.set(x, (y + 1));
		if (getAquarium()->isOpen(p) && m_discovered.find(p) == -1) {
			m_look.push(p);
			m_discovered.push_rear(p);
		}
		p = currentPosition();

		if (m_look.empty() == true)
			setState(State::STUCK);
		else if (currentPosition() == getAquarium()->getEndPoint())
			setState(State::FREEDOM);
	}
}