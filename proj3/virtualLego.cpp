////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <math.h>

IDirect3DDevice9* Device = NULL;

// window size
const int Width  = 1080;
const int Height = 760;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)
#define ball_num 39
const float spherePos[ball_num][2] = { {-2.9f,1.7},{-2.4f,1.7},{-1.9f,1.7}, {-3.3f,1.3},{-3.3f,0.7},{-3.3f,0.2},{-3.3f,-0.3}, {-1.5f,1.3},{-1.5f,0.7},{-1.5f,0.2},{-1.5f,-0.3}, {-2.9f,-0.7},{-2.4f,-0.7},{-1.9f,-0.7},
	                                   {-0.5f,1.7},{0.0f,1.7},{0.50f,1.7}, {-0.93f,1.3},{-0.93f,0.7},{-0.93f,0.2},{-0.93f,-0.3}, {0.95f,1.3},{0.95f,0.7},{0.95f,0.2},{0.95f,-0.3}, {-0.5f,-0.7},{0.0f,-0.7},{0.50f,-0.7},
									   {2.03f,1.7},{2.48f,1.7},{2.93f,1.7}, {1.61f,1.3},{1.61f,0.7},{1.61f,0.2},{1.61f,-0.3},{1.61f,-0.8}, {3.3f,1.3} ,{2.2f,0.7},{2.8f,0.7}};
// initialize the color of each ball (ball0 ~ ball3)
D3DXCOLOR sphereColor[ball_num] = { };

bool pressed_space = false;

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private :
	float					center_x, center_y, center_z;
    float                   m_radius;
	float					m_velocity_x;
	float					m_velocity_z;

public:
	bool is_control_ball = false; // to distinguish control ball

    CSphere(void)
    {
        D3DXMatrixIdentity(&m_mLocal);
        ZeroMemory(&m_mtrl, sizeof(m_mtrl));
        m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
        m_pSphereMesh = NULL;
    }
    ~CSphere(void) {}
	
public:
    bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
    {
        if (NULL == pDevice)
            return false;
		
        m_mtrl.Ambient  = color;
        m_mtrl.Diffuse  = color;
        m_mtrl.Specular = color;
        m_mtrl.Emissive = d3d::BLACK;
        m_mtrl.Power    = 5.0f;
		
        if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
            return false;
        return true;
    }
	
    void destroy(void)
    {
        if (m_pSphereMesh != NULL) {
            m_pSphereMesh->Release();
            m_pSphereMesh = NULL;
        }
    }

    void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
    {
        if (NULL == pDevice)
            return;
        pDevice->SetTransform(D3DTS_WORLD, &mWorld);
        pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
        pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
    }
	
    bool hasIntersected(CSphere& ball) 
	{
		// Insert your code here.
		float diff_x, diff_z;

		diff_x = this->center_x - ball.center_x;
		diff_z = this->center_z - ball.center_z;
		if (sqrt(diff_x * diff_x + diff_z * diff_z) <= ball.getRadius() + this->getRadius()) // intersected when distance between centers is less or equal to sum of radius of two spheres+
			return true;
		return false;  
	}
	
	void hitBy(CSphere& ball) 
	{ 
		// Insert your code here.
		
		if (hasIntersected(ball) == true) {
					
			//float delta_x = ball.getCenter().x - this->getCenter().x;

			float delta_x = (ball.getCenter().x - this->getCenter().x);
			float delta_z = (ball.getCenter().z - this->getCenter().z);
	
			float speed = sqrt(pow(ball.getVelocity_X(), 2) + pow(ball.getVelocity_Z(), 2));
			float distance = sqrt(delta_x * delta_x + delta_z * delta_z);  
			float delta_time = distance / speed;
		
			ball.setPower(delta_x / delta_time, delta_z / delta_time);
			
			if (is_control_ball == false) {
				this->setCenter(999.0f, 999.0f, -999.0f);
			}			
		}
	}

	void ballUpdate(float timeDiff) 
	{
		const float TIME_SCALE = 3.3;  
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if(vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
			float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;
			printf("%f", timeDiff);
			//correction of position of ball
			// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
			if(tX >= (3.8 - M_RADIUS))
				tX = 3.8 - M_RADIUS;
			else if(tX <=(-3.8 + M_RADIUS))
				tX = -3.8 + M_RADIUS;
			//else if(tZ <= (-3 + M_RADIUS))
				//tZ = -3 + M_RADIUS;
			else if(tZ >= (3 - M_RADIUS))
				tZ = 3 - M_RADIUS;
			
			this->setCenter(tX, cord.y, tZ);
		}		
		else { this->setPower(0, 0); }

		/*
		//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
		double rate = 1 - (1 - DECREASE_RATE) * timeDiff * 400;
		if(rate < 0 )
			rate = 0;
		this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
		*/
	}

	double getVelocity_X() { return this->m_velocity_x;	}
	double getVelocity_Z() { return this->m_velocity_z; }

	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x=x;	center_y=y;	center_z=z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	
	float getRadius(void)  const { return (float)(M_RADIUS);  }
    const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
    void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
    D3DXVECTOR3 getCenter(void) const
    {
        D3DXVECTOR3 org(center_x, center_y, center_z);
        return org;
    }
	
private:
    D3DXMATRIX              m_mLocal;
    D3DMATERIAL9            m_mtrl;
    ID3DXMesh*              m_pSphereMesh;
	
};



// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:
	
    float					m_x;
	float					m_z;
	float                   m_width;
    float                   m_depth;
	float					m_height;

	
	float getWidth(void) const { return m_width; }
	float getDepth(void) const { return m_depth; }
	
public:
    CWall(void)
    {
        D3DXMatrixIdentity(&m_mLocal);
        ZeroMemory(&m_mtrl, sizeof(m_mtrl));
        m_width = 0;
        m_depth = 0;
        m_pBoundMesh = NULL;
    }
    ~CWall(void) {}
public:
    bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
    {
        if (NULL == pDevice)
            return false;
		
        m_mtrl.Ambient  = color;
        m_mtrl.Diffuse  = color;
        m_mtrl.Specular = color;
        m_mtrl.Emissive = d3d::BLACK;
        m_mtrl.Power    = 5.0f;
		
        m_width = iwidth;
        m_depth = idepth;
		
        if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
            return false;
        return true;
    }
    void destroy(void)
    {
        if (m_pBoundMesh != NULL) {
            m_pBoundMesh->Release();
            m_pBoundMesh = NULL;
        }
    }
    void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
    {
        if (NULL == pDevice)
            return;
        pDevice->SetTransform(D3DTS_WORLD, &mWorld);
        pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
        pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
    }
	
	bool hasIntersected(CSphere& ball) 
	{	
		// Insert your code here.
		
		float right_limit = this->m_x - this->getWidth() / 2 - M_RADIUS;
		float left_limit = this->m_x + this->getWidth() / 2 + M_RADIUS;
		float top_limit = this->m_z - this->getDepth() / 2 - M_RADIUS;

		if ((ball.getCenter().x == right_limit) || (ball.getCenter().x == left_limit) || (ball.getCenter().z >= top_limit))
			return true; 
		return false; 
		
		
	}

	void hitBy(CSphere& ball) 
	{	
		// Insert your code here.
		
		if (hasIntersected(ball) == true) {
			float right_limit = this->m_x - this->getWidth() / 2 - ball.getRadius();
			float left_limit = this->m_x + this->getWidth() / 2 + ball.getRadius();
			float top_limit = this->m_z - this->getDepth() / 2 - ball.getRadius();
			
			if (ball.getCenter().z == top_limit) {
				ball.setPower(ball.getVelocity_X(), -ball.getVelocity_Z());				
			} 

			if (ball.getCenter().x == right_limit) {
				ball.setPower(-ball.getVelocity_X(), ball.getVelocity_Z());				
			}

			if (ball.getCenter().x == left_limit) {
				ball.setPower(-ball.getVelocity_X(), ball.getVelocity_Z());
			}

		}
	}    
	
	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	
    float getHeight(void) const { return M_HEIGHT; }
	
	
	
private :
    void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	
	D3DXMATRIX              m_mLocal;
    D3DMATERIAL9            m_mtrl;
    ID3DXMesh*              m_pBoundMesh;
};

// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

class CLight {
public:
    CLight(void)
    {
        static DWORD i = 0;
        m_index = i++;
        D3DXMatrixIdentity(&m_mLocal);
        ::ZeroMemory(&m_lit, sizeof(m_lit));
        m_pMesh = NULL;
        m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_bound._radius = 0.0f;
    }
    ~CLight(void) {}
public:
    bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
    {
        if (NULL == pDevice)
            return false;
        if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
            return false;
		
        m_bound._center = lit.Position;
        m_bound._radius = radius;
		
        m_lit.Type          = lit.Type;
        m_lit.Diffuse       = lit.Diffuse;
        m_lit.Specular      = lit.Specular;
        m_lit.Ambient       = lit.Ambient;
        m_lit.Position      = lit.Position;
        m_lit.Direction     = lit.Direction;
        m_lit.Range         = lit.Range;
        m_lit.Falloff       = lit.Falloff;
        m_lit.Attenuation0  = lit.Attenuation0;
        m_lit.Attenuation1  = lit.Attenuation1;
        m_lit.Attenuation2  = lit.Attenuation2;
        m_lit.Theta         = lit.Theta;
        m_lit.Phi           = lit.Phi;
        return true;
    }
    void destroy(void)
    {
        if (m_pMesh != NULL) {
            m_pMesh->Release();
            m_pMesh = NULL;
        }
    }
    bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
    {
        if (NULL == pDevice)
            return false;
		
        D3DXVECTOR3 pos(m_bound._center);
        D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
        D3DXVec3TransformCoord(&pos, &pos, &mWorld);
        m_lit.Position = pos;
		
        pDevice->SetLight(m_index, &m_lit);
        pDevice->LightEnable(m_index, TRUE);
        return true;
    }

    void draw(IDirect3DDevice9* pDevice)
    {
        if (NULL == pDevice)
            return;
        D3DXMATRIX m;
        D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
        pDevice->SetTransform(D3DTS_WORLD, &m);
        pDevice->SetMaterial(&d3d::WHITE_MTRL);
        m_pMesh->DrawSubset(0);
    }

    D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

private:
    DWORD               m_index;
    D3DXMATRIX          m_mLocal;
    D3DLIGHT9           m_lit;
    ID3DXMesh*          m_pMesh;
    d3d::BoundingSphere m_bound;
};


// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
CWall	g_legowall[3]; // num of walls
CSphere	g_sphere[ball_num]; // num of balls
CSphere	control_ball;
CSphere	bullet_ball;
CLight	g_light;

double g_camera_pos[3] = {0.0, 5.0, -8.0};

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{

}

// initialization
bool Setup()
{
	int i;
	
    D3DXMatrixIdentity(&g_mWorld);
    D3DXMatrixIdentity(&g_mView);
    D3DXMatrixIdentity(&g_mProj);
		
	// create plane and set the position
    if (false == g_legoPlane.create(Device, -1, -1, 7.6, 0.03f, 6, d3d::GREEN)) return false;
    g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);
	
	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 7.6, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	/*if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);*/
	if (false == g_legowall[1].create(Device, -1, -1, 0.12f, 0.3f, 6.12f, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(3.86f, 0.12f, 0.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.12f, d3d::DARKRED)) return false;
	g_legowall[2].setPosition(-3.86f, 0.12f, 0.06f);
	
	std::fill_n(sphereColor, ball_num, d3d::YELLOW); // set the color of brick balls in yellow


	// create balls and set the position
	for (i=0;i< ball_num;i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS , spherePos[i][1]);
		g_sphere[i].setPower(0,0);
	}
	
	// create white ball for control
    if (false == control_ball.create(Device, d3d::WHITE)) return false;
	control_ball.setCenter(0.0f, (float)M_RADIUS, -3.0f);
	control_ball.is_control_ball = true;
	
	// create red bullet ball 
	if (false == bullet_ball.create(Device, d3d::RED)) return false;
	bullet_ball.setCenter(0.0f, (float)M_RADIUS, -3.21f + 2*M_RADIUS);

	// light setting 
    D3DLIGHT9 lit;
    ::ZeroMemory(&lit, sizeof(lit));
    lit.Type         = D3DLIGHT_POINT;
    lit.Diffuse      = d3d::WHITE; 
	lit.Specular     = d3d::WHITE * 0.9f;
    lit.Ambient      = d3d::WHITE * 0.9f;
    lit.Position     = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
    lit.Range        = 100.0f;
    lit.Attenuation0 = 0.0f;
    lit.Attenuation1 = 0.9f;
    lit.Attenuation2 = 0.0f;
    if (false == g_light.create(Device, lit))
        return false;
	
	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);
	
	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
        (float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
	
    // Set render states.
    Device->SetRenderState(D3DRS_LIGHTING, TRUE);
    Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	
	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
    g_legoPlane.destroy();
	for(int i = 0 ; i < 3; i++) {
		g_legowall[i].destroy();
	}
    destroyAllLegoBlock();
    g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i = 0;
	int j = 0;


	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		if (pressed_space == false) { // before space bar is pressed, bullet ball follows control ball
			bullet_ball.setCenter(control_ball.getCenter().x, control_ball.getCenter().y, control_ball.getCenter().z + 2 * M_RADIUS);
		}

		// update the position of each ball. during update, check whether each ball hit by walls.	
		for( i = 0; i < ball_num; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for(j = 0; j < 3; j++){
				g_legowall[j].hitBy(bullet_ball); }
		}
		bullet_ball.ballUpdate(timeDelta);

		// check whether each ball hit by control ball and update the direction of balls
		for(i = 0 ;i < ball_num; i++){
				g_sphere[i].hitBy(bullet_ball);
		}
		 
		control_ball.hitBy(bullet_ball); // check whether control ball hit by moving ball.
		
		if (bullet_ball.getCenter().z <= -3.21f) { // if bullet ball goes out of plane, bullet ball follows control ball again
			pressed_space = false;
			bullet_ball.setPower(0, 0);						
		}
		
		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i=0; i < 3; i++) 	{ // draw walls
			g_legowall[i].draw(Device, g_mWorld);
		}
		for (i = 0; i < ball_num; i++) { // draw spheres
			g_sphere[i].draw(Device, g_mWorld);
		}
		control_ball.draw(Device, g_mWorld);
		bullet_ball.draw(Device, g_mWorld);
        g_light.draw(Device);
		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture( 0, NULL );
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
    static int old_x = 0;
    static int old_y = 0;
    static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;
	
	switch( msg ) {
	case WM_DESTROY:
        {
			::PostQuitMessage(0);
			break;
        }
	case WM_KEYDOWN:
        {
            switch (wParam) {
            case VK_ESCAPE:
				::DestroyWindow(hwnd);
                break;
            case VK_RETURN:
                if (NULL != Device) {
                    wire = !wire;
                    Device->SetRenderState(D3DRS_FILLMODE,
                        (wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
                }
                break;
            case VK_SPACE:
				/*
				D3DXVECTOR3 targetpos = control_ball.getCenter();
				D3DXVECTOR3	whitepos = g_sphere[4].getCenter();
				double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
					pow(targetpos.z - whitepos.z, 2)));		// 기본 1 사분면
				if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }	//4 사분면
				if (targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 사분면
				if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0){ theta = PI + theta; } // 3 사분면
				double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
				g_sphere[4].setPower(distance * cos(theta), distance * sin(theta));
				*/

				if (pressed_space == false)
				{
					pressed_space = true;
					bullet_ball.setPower(0.0, 2.0f); // start the game by pressing SPACE key, which will shoot the red ball upward.
				}
				break;

			}
			break;
        }
		
	case WM_MOUSEMOVE:
        {
            int new_x = LOWORD(lParam);
            int new_y = HIWORD(lParam);
			float dx;
			float dy;
			
            if (LOWORD(wParam) & MK_LBUTTON) {
				
                if (isReset) {
                    isReset = false;
                } else {
                    D3DXVECTOR3 vDist;
                    D3DXVECTOR3 vTrans;
                    D3DXMATRIX mTrans;
                    D3DXMATRIX mX;
                    D3DXMATRIX mY;
					
                    switch (move) {
                    case WORLD_MOVE:
                        dx = (old_x - new_x) * 0.01f;
                        dy = (old_y - new_y) * 0.01f;
                        D3DXMatrixRotationY(&mX, dx);
                        D3DXMatrixRotationX(&mY, dy);
                        g_mWorld = g_mWorld * mX * mY;
						
                        break;
                    }
                }
				
                old_x = new_x;
                old_y = new_y;

            } else {
                isReset = true;
				
				if (LOWORD(wParam) & MK_RBUTTON) {
					dx = (old_x - new_x);// * 0.01f;
					//dy = (old_y - new_y);// * 0.01f;
		
					D3DXVECTOR3 coord3d=control_ball.getCenter();
					//control_ball.setCenter(coord3d.x+dx*(-0.007f),coord3d.y,coord3d.z+dy*0.007f );
					control_ball.setCenter(coord3d.x + dx * (-0.007f), coord3d.y, -3.0f);

				}
				old_x = new_x;
				//old_y = new_y;
				
                move = WORLD_MOVE;
            }
            break;
        }
	}
	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
    srand(static_cast<unsigned int>(time(NULL)));
	
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	
	d3d::EnterMsgLoop( Display );
	
	Cleanup();
	
	Device->Release();
	
	return 0;
}