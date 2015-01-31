#include "..\header\Player.h"

Player::Player(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	ResourceManager& r = ResourceManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	if (!l.isStarted() || !r.isStarted() || !w.isStarted()){
		l.writeLog("[Player] Manager was not started. Order by: %s %s %s", BoolToString(l.isStarted()), BoolToString(r.isStarted()), BoolToString(w.isStarted()));
		w.markForDelete(this);
		return;
	}

	//No need for a sprite. A simple character will do.
	setType(TYPE_PLAYER);
	setTransparency();
	setSolidness(Solidness::HARD);
	Position pos(GameState->PlayerState.initialX, GameState->PlayerState.initialY);
	setPosition(pos);

	registerInterest(DF_KEYBOARD_EVENT);
	registerInterest(DF_STEP_EVENT);

	this->GameState = GameState;
	setVisible(true);
	l.writeLog("[Player] Successfully loaded Player entity.");
}


//TODO(Thompson): Start working on core mechanics (Rotation!)
int Player::eventHandler(Event* e){
	LogManager& l = LogManager::getInstance();
	if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		switch (key){
			case 'a':{
				int x = this->getPosition().getX();
				int y = this->getPosition().getY();
				int layoutX = (x - 2) - this->GameState->PlayerState.minX;
				int layoutY = this->getPosition().getY() - this->GameState->PlayerState.minY - 1;
				if (layoutX >= 0){
					int width = this->GameState->Stage1.width;
					int* layout = this->GameState->Stage1.layout;
					if (layoutY*width + layoutX > 0){
						int check = *(layout + (layoutY *width + layoutX));
						if (check == 0){
							x--;
						}
					}
					else {
						l.writeLog("[Player] Error calculating X and Y, size limit is 286, actual value: %d", layoutY*width + layoutX);
					}
				}
				this->setPosition(Position(x, this->getPosition().getY()));
				this->GameState->PlayerState.x = layoutX;
				this->GameState->PlayerState.y = layoutY;
				break;
			}
			case 'd':{
				int x = this->getPosition().getX();
				int layoutX = (x) -this->GameState->PlayerState.minX;
				int width = this->GameState->Stage1.width;
				int layoutY = this->getPosition().getY() - this->GameState->PlayerState.minY - 1;
				if (layoutX < width){
					int* layout = this->GameState->Stage1.layout;
					if (layoutY*width + layoutX < 286){
						int check = *(layout + (layoutY *width + layoutX));
						if (check == 0){
							x++;
						}
					}
					else {
						l.writeLog("[Player] Error calculating X and Y, size limit is 286, actual value: %d", layoutY*width + layoutX);
					}
				}
				this->setPosition(Position(x, this->getPosition().getY()));
				this->GameState->PlayerState.x = layoutX;
				this->GameState->PlayerState.y = layoutY;
				break;
			}
			case 'q':{
				l.writeLog("This is Q. Rotates the arena counterclockwise. ");
				this->GameState->Board.rotateCCW = true;
				this->GameState->Board.rotateCW = false;
				this->GameState->Stage1.layout[this->GameState->PlayerState.y * this->GameState->Stage1.width + this->GameState->PlayerState.x] = 999;
				break;
			}
			case 'e': {
				l.writeLog("This is E. Rotates the arena clockwise. ");
				this->GameState->Board.rotateCW = true;
				this->GameState->Board.rotateCCW = false;
				this->GameState->Stage1.layout[this->GameState->PlayerState.y * this->GameState->Stage1.width + this->GameState->PlayerState.x] = 999;
				break;
			}
			default:{
				return 0;
			}
		}
		return 1;
	}
	else if (e->getType() == DF_STEP_EVENT){
		if (this->GameState->Board.isRotating){
			return 1;
		}
		//Current location
		int x = this->getPosition().getX();
		int y = this->getPosition().getY();
		int layoutY = (y - this->GameState->PlayerState.minY - 1);
		int layoutX = (x - this->GameState->PlayerState.minX - 1);
		int* layout = this->GameState->Stage1.layout;

		if (this->GameState && this->GameState->Stage1.layout){
			//Gravity affected movement.
			//l.writeLog("Current Order: %d", this->GameState->Board.arrayOrder);

			//TODO(Thompson): Make all of the gravity movements be matching with 1 coordinates system.


			switch (this->GameState->Board.arrayOrder){
				case 0:{
					//int layoutY = (y - this->GameState->PlayerState.minY - 1);
					////l.writeLog("Player abs pos: %d, %d  -----  minX, minY, maxX, maxY:  %d, %d, %d, %d", x, y, this->GameState->PlayerState.minX, this->GameState->PlayerState.minY, this->GameState->PlayerState.maxX, this->GameState->PlayerState.maxY);
					////l.writeLog("layoutX: %d", layoutY);
					//if (layoutY + 1 < this->GameState->Stage1.height){
					//	int layoutX = (x - this->GameState->PlayerState.minX - 1);
					//	int width = this->GameState->Stage1.width;
					//	int* layout = this->GameState->Stage1.layout;
					//	int check = *(layout + ((layoutY + 1)*width + layoutX));
					//	if (check == 0 && check < 10 && check > -1){
					//		y++;
					//	}
					//	//l.writeLog("layoutX, layoutY, check: %d, %d, %d", layoutX, layoutY, check);
					//}
					if (layoutY + 1 < this->GameState->Stage1.height){
						if (layout[(layoutY + 1)*this->GameState->Stage1.width + layoutX] == 0){
							y++;
						}
					}
					break;
				}
				case 1:{
					//int layoutX = (x - this->GameState->PlayerState.minX - 1);
					///*l.writeLog("Player abs pos: %d, %d  -----  minX, minY, maxX, maxY:  %d, %d, %d, %d", x, y, this->GameState->PlayerState.minX, this->GameState->PlayerState.minY, this->GameState->PlayerState.maxX, this->GameState->PlayerState.maxY);
					//l.writeLog("layoutX: %d", layoutX);*/
					//if (layoutX + 1 < this->GameState->Stage1.width){
					//	int layoutY = (y - this->GameState->PlayerState.minY - 1);
					//	int width = this->GameState->Stage1.width;
					//	int check = this->GameState->Stage1.layout[layoutY*width + (layoutX+1)];
					//	if (check == 0){
					//		x++;
					//	}
					//	//l.writeLog("layoutX, layoutY, check: %d, %d, %d", layoutX, layoutY, check);
					//}



					break;
				}
				case 2:{
					/*int layoutY = (this->GameState->PlayerState.maxY - y);
					l.writeLog("Player abs pos: %d, %d  -----  minX, minY, maxX, maxY:  %d, %d, %d, %d", x, y, this->GameState->PlayerState.minX, this->GameState->PlayerState.minY, this->GameState->PlayerState.maxX, this->GameState->PlayerState.maxY);
					l.writeLog("layoutY: %d", layoutY);
					if (layoutY >= 0){
						int layoutX = (this->GameState->PlayerState.maxX - x);
						int width = this->GameState->Stage1.width;
						int* layout = this->GameState->Stage1.layout;
						int check = *(layout + ((layoutY + 1)*width + layoutX));
						if (check == 0 && check < 10 && check > -1){
							y--;
						}
						l.writeLog("layoutX, layoutY, check: %d, %d, %d", layoutX, layoutY, check);
					}*/
					break;
				}
				case 3:{
					//int layoutX = (x - this->GameState->PlayerState.minX - 1);
					////l.writeLog("Player abs pos: %d, %d  -----  minX, minY, layoutX: %d, %d, %d", x, y, this->GameState->PlayerState.minX, this->GameState->PlayerState.minX, layoutX);
					//if (layoutX >= 0){
					//	int layoutY = (y - this->GameState->PlayerState.minY - 1);
					//	int width = this->GameState->Stage1.width;
					//	int check = this->GameState->Stage1.layout[layoutY*width + (layoutX - 1)];
					//	if (check == 0){
					//		x--;
					//	}
					//	//l.writeLog("layoutY, check: %d, %d", layoutY, check);
					//}
					break;
				}
			}
		}

		//Checks to see if the location is within bounds.
		if (x <= this->GameState->PlayerState.minX){
			x = this->GameState->PlayerState.minX + 1;
		}
		if (y <= this->GameState->PlayerState.minY){
			y = this->GameState->PlayerState.minY + 1;
		}
		if (x > this->GameState->PlayerState.maxX){
			x = this->GameState->PlayerState.maxX;
		}
		if (y > this->GameState->PlayerState.maxY){
			y = this->GameState->PlayerState.maxY;
		}

		//Set new position.
		this->GameState->PlayerState.x = x;
		this->GameState->PlayerState.y = y;
		this->setPosition(Position(x, y));
		return 1;
	}
	return 0;
}

void Player::draw(){
	if (this->GameState->Board.isRotating){
		return;
	}
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	int width = this->GameState->Stage1.width;
	int height = this->GameState->Stage1.height;
	Position pos = this->getPosition();
	switch (this->GameState->Board.arrayOrder){
		case 0:{
			g.drawCh(pos, '@', 0);
			break; 
		}
		case 1:{
			int x = this->GameState->PlayerState.x;
			int newY = this->GameState->PlayerState.minY + (x - this->GameState->PlayerState.minX);
			int y = this->GameState->PlayerState.y;
			int newX = this->GameState->PlayerState.minX + (this->GameState->PlayerState.maxY - y + 1);
			Position newPos(newX, newY);
			g.drawCh(newPos, '@', 0);
			break;
		}
		case 2:{
			int x = this->GameState->PlayerState.x;
			int newX = this->GameState->PlayerState.maxX - (x - this->GameState->PlayerState.minX - 1);
			int y = this->GameState->PlayerState.y;
			int newY = this->GameState->PlayerState.minY + (this->GameState->PlayerState.maxY - y + 1);
			Position newPos(newX, newY);
			g.drawCh(newPos, '@', 0);
			break;
		}
		case 3:{
			int x = this->GameState->PlayerState.x;
			int newY = this->GameState->PlayerState.maxY - (x - this->GameState->PlayerState.minX - 1);
			int y = this->GameState->PlayerState.y;
			int newX = this->GameState->PlayerState.maxX - (this->GameState->PlayerState.maxY - y);
			Position newPos(newX, newY);
			g.drawCh(newPos, '@', 0);
			break; 
		}
		default:{
			l.writeLog("[Player] Wrong array order: %d", (int)(this->GameState->Board.arrayOrder));
			break; 
		}
	}
	
	return;
}

void Player::initializeState(game_state* GameState){
	this->GameState = GameState;
	this->setPosition(Position(GameState->PlayerState.x, GameState->PlayerState.y));
	return;
}

void Player::setGameBounds(int x, int y, int w, int h){
	if (this->GameState){
		this->GameState->PlayerState.minX = x;
		this->GameState->PlayerState.minY = y;
		this->GameState->PlayerState.maxX = w;
		this->GameState->PlayerState.maxY = h;
		LogManager& l = LogManager::getInstance();
		l.writeLog("[Player] Setting game boundaries for left, top, width, and height: %d, %d, %d, %d", x, y, w, h);
	}
}