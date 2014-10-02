#include "Myracer.h"

// Declare our game instance
Myracer game;

Myracer::Myracer()
: _scene(NULL), _wireframe(false), _driveIt(false), _steering(0), _carVehicle(NULL)
{
}

void Myracer::initialize()
{
    // Load game scene from file
    _scene = Scene::load("res/demo.scene");

	// Set the aspect ratio for the scene's camera to match the current resolution
	_scene->getActiveCamera()->setAspectRatio(getAspectRatio());
	_scene->getActiveCamera()->getNode()->rotateY(MATH_DEG_TO_RAD(-90)); //dd

	// Initialize scene
	_scene->visit(this, &Myracer::initializeScene);

	// Load and initialize game script   dd attempt to imitate supplied example that uses LUA
	//getScriptController()->loadScript("res/racer.lua");
	//getScriptController()->executeFunction<void>("setScene", "<Scene>", _scene);

    // Get the box model and initialize its material parameter values and bindings
    Node* carNode = _scene->findNode("carbody"); //dd ("box")
    Model* boxModel = carNode->getModel();
    Material* boxMaterial = boxModel->getMaterial();

	Node* wheelFLnode = _scene->findNode("wheelFrontLeft");
	Node* wheelFRnode = _scene->findNode("wheelFrontRight");
	Node* wheelBLnode = _scene->findNode("wheelBackLeft");
	Node* wheelBRnode = _scene->findNode("wheelBackRight");

	//create parent node as suggested by andrewRN on Forum (now no parenting in Blender)
	Node* vehicleParentNode = _scene->addNode();
	vehicleParentNode->setTranslation(carNode->getTranslation());

	vehicleParentNode->addChild(carNode);
	vehicleParentNode->addChild(wheelFLnode);
	vehicleParentNode->addChild(wheelFRnode);
	vehicleParentNode->addChild(wheelBRnode);
	vehicleParentNode->addChild(wheelBLnode);

	//  /************** does not appear to amek a difference if wheel collision objects defined
	if (wheelFLnode && wheelFLnode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE_WHEEL)
	{
		_wheelFL = static_cast<PhysicsVehicleWheel*>(wheelFLnode->getCollisionObject());
	}
	if (wheelFRnode && wheelFRnode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE_WHEEL)
	{
		_wheelFR = static_cast<PhysicsVehicleWheel*>(wheelFRnode->getCollisionObject());
	}

	if (wheelBLnode && wheelBLnode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE_WHEEL)
	{
		_wheelBL = static_cast<PhysicsVehicleWheel*>(wheelBLnode->getCollisionObject());
	}
	if (wheelBRnode && wheelBRnode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE_WHEEL)
	{
		_wheelBR = static_cast<PhysicsVehicleWheel*>(wheelBRnode->getCollisionObject());
	}
	//  ***************/
	if (carNode && carNode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE)
	{
		_carVehicle = static_cast<PhysicsVehicle*>(carNode->getCollisionObject());
		_carVehicle->setEnabled(false);
		_carVehicle->reset();
		_carVehicle->setEnabled(true);
	}

}

bool Myracer::initializeScene(Node* node)
{
	//static Node* lightNode = _scene->findNode("directionalLight1");

	Model* model = node->getModel();
	if (model)
	{
		Material* material = model->getMaterial();
		/*******
		if (material && material->getTechnique()->getPassByIndex(0)->getEffect()->getUniform("u_directionalLightDirection"))
		{
			material->getParameter("u_ambientColor")->setValue(_scene->getAmbientColor());
			material->getParameter("u_directionalLightColor[0]")->setValue(lightNode->getLight()->getColor());
			material->getParameter("u_directionalLightDirection[0]")->setValue(lightNode->getForwardVectorView());
		} ****/
	}
		
	return true;
}

void Myracer::finalize()
{
    SAFE_RELEASE(_scene);
}

void Myracer::update(float elapsedTime)
{
    // Rotate model
    //_scene->findNode("track")->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 1000.0f * 180.0f));
	if (_carVehicle && _driveIt) {
		_carVehicle->update(elapsedTime, 0.0f, 0.0f, 1.0f);  //steer -1 to 1, brake 0 to 1, drive 0 to 1
	}
}

void Myracer::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Myracer::drawScene);
}

bool Myracer::drawScene(Node* node)
{
    // If the node visited contains a model, draw it
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw(_wireframe);
    }
    return true;
}

void Myracer::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
		case Keyboard::KEY_D:
			_driveIt = true;
			break;
		case Keyboard::KEY_CAPITAL_D:
			_driveIt = false;
			break;
		}
    }
}

void Myracer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        _wireframe = !_wireframe;
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
