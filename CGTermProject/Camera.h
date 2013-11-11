#ifndef H_CAMERA
#define H_CAMERA

class Camera
{
public:
	Camera(
		float posX = 0.0, 
		float posY = 5.0, 
		float posZ = 30.0, 
		float frustumWidth = 10.0,
		float frustumHeight = 10.0,
		float frustumNear = 5.0,
		float frustumFar = 100.0,
		float windowWidth = 500,
		float windowHeight = 500);
	~Camera();

	float getPosX() { return m_posX; }
	float getPosY() { return m_posY; }
	float getPosZ() { return m_posZ; }

	float getCenterX() { return m_centerX; }
	float getCenterY() { return m_centerY; }
	float getCenterZ() { return m_centerZ; }

	float getFrustumWidth() { return m_frustumWidth; }
	float getFrustumHeight() { return m_frustumHeight; }
	float getFrustumHalfWidth() { return m_frustumHalfWidth; }
	float getFrustumHalfHeight() { return m_frustumHalfHeight; }
	float getFrustumNear() { return m_frustumNear; }
	float getFrustumFar() { return m_frustumFar; }

	float getWindowWidth() { return m_windowWidth; }
	float getWindowHeight() { return m_windowHeight; }
	float getWindowHalfWidth() { return m_windowHalfWidth; }
	float getWindowHalfHeight() { return m_windowHalfHeight; }

	void setWindowWidth( float windowWidth ) { m_windowWidth = windowWidth; m_windowHalfWidth = windowWidth / 2; }
	void setWindowHeight( float windowHeight ) { m_windowHeight = windowHeight; m_windowHalfHeight = windowHeight / 2; };

private:
	// camera location
	float m_posX;
	float m_posY;
	float m_posZ;

	// camera focus, no tilt for now
	float m_centerX; // for now, must look straight ahead
	float m_centerY; // for now, must look straight ahead
	float m_centerZ;

	// frustum
	float m_frustumWidth;
	float m_frustumHeight;
	float m_frustumHalfWidth;
	float m_frustumHalfHeight;
	float m_frustumNear;
	float m_frustumFar;

	// gl window
	float m_windowWidth;
	float m_windowHeight;
	float m_windowHalfWidth;
	float m_windowHalfHeight;
};

#endif