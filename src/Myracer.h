#ifndef Myracer_H_
#define Myracer_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class Myracer: public Game
{
public:

    /**
     * Constructor.
     */
    Myracer();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:
	/**
	* Initializes the scene.
	*/
	bool initializeScene(Node* node);

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node);

    Scene* _scene;
    bool _wireframe;
	bool _driveIt;

	//dd for racer
	float _steering;
	PhysicsVehicle* _carVehicle;
	PhysicsVehicleWheel* _wheelFL;
	PhysicsVehicleWheel* _wheelFR;
	PhysicsVehicleWheel* _wheelBL;
	PhysicsVehicleWheel* _wheelBR;
};

#endif
