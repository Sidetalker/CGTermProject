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

	float getPosX() const { return m_posX; }
	float getPosY() const { return m_posY; }
	float getPosZ() const { return m_posZ; }

	float getCenterX() const { return m_centerX; }
	float getCenterY() const { return m_centerY; }
	float getCenterZ() const { return m_centerZ; }

	float getFrustumWidth() const { return m_frustumWidth; }
	float getFrustumHeight() const { return m_frustumHeight; }
	float getFrustumHalfWidth() const { return m_frustumHalfWidth; }
	float getFrustumHalfHeight() const { return m_frustumHalfHeight; }
	float getFrustumNear() const { return m_frustumNear; }
	float getFrustumFar() const { return m_frustumFar; }

	float getWindowWidth() const { return m_windowWidth; }
	float getWindowHeight() const { return m_windowHeight; }
	float getWindowHalfWidth() const { return m_windowHalfWidth; }
	float getWindowHalfHeight() const { return m_windowHalfHeight; }

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