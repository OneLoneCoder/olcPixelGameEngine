/*
	Programming a robotic arm
	"I told you, put down the screwdriver..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Instructions:
	~~~~~~~~~~~~~
	Without a robot arm and an mbed there is not much you can do!
	Also requires a 3rd Party PGEX UI by ZleapingBear:
	https://youtu.be/bfiSjC__MCI
	

	Relevant Video: https://youtu.be/ekdQ-aAB36Y

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "olcPGEX_UI.h"

class RobotArm1 : public olc::PixelGameEngine
{
public:
	RobotArm1()
	{
		sAppName = "Robot Arm 1";
	}

	olc::UI_CONTAINER gui;
	float fJointAngle[6];
	float fAccumulatedTime = 0.0f;
	HANDLE hCom = nullptr;


public:
	bool OnUserCreate() override
	{
		gui.addSlider(10, 20, 180);
		gui.addSlider(10, 60, 180);
		gui.addSlider(10, 100, 180);
		gui.addSlider(10, 140, 180);
		gui.addSlider(10, 180, 180);
		gui.addSlider(10, 220, 180);
		gui.setValue(0, 50);
		gui.setValue(1, 50);
		gui.setValue(2, 50);
		gui.setValue(3, 50);
		gui.setValue(4, 50);
		gui.setValue(5, 50);

		// Open COM Port
		hCom = CreateFile("COM3", GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hCom == nullptr) return false;

		// Configure Protocol: 9600bps, 8N1
		DCB dcb = { 0 };
		GetCommState(hCom, &dcb);
		dcb.BaudRate = CBR_9600;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;
		SetCommState(hCom, &dcb);		
		return true;
	}

	bool OnUserDestroy() override
	{
		if (hCom != nullptr) CloseHandle(hCom);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		gui.Update(fElapsedTime);
		Clear(olc::GREEN);
		gui.drawUIObjects();

		for (int i = 0; i < 6; i++)
			fJointAngle[i] = (gui.getSliderFloat(i) / 100.0f) * 180.0f - 90.0f;

		unsigned char command[12];
		for (int i = 0; i < 6; i++)
		{
			command[i * 2 + 0] = i;
			command[i * 2 + 1] = (int)(128 + fJointAngle[i]);
		}

		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime > 0.05f)
		{
			fAccumulatedTime -= 0.05f;
			DWORD bw = 0;
			WriteFile(hCom, command, 12, &bw, 0);
		}

		return true;
	}
};

int main()
{
	RobotArm1 demo;
	if (demo.Construct(400, 400, 2, 2))
		demo.Start();
	return 0;
}

// Below here is the source code compiled on MBED LPC1768, using the BufferedSerial Library

/*

#include "mbed.h"
#include "BufferedSerial.h"

PwmOut pin26(p26);
PwmOut pin25(p25);
PwmOut pin24(p24);
PwmOut pin23(p23);
PwmOut pin22(p22);
PwmOut pin21(p21);

BufferedSerial uart(p9, p10);


class Joint
{
private:
    static const float fDutyMin = 0.03f; // -90
    static const float fDutyMax = 0.11f; // +90
    static const float fDutyRange = fDutyMax - fDutyMin;
    
    float fTarget;
    float fPosition;
    float fJointMax;
    float fJointMin;
    
public: 
    Joint(float fMin = -90.0f, float fMax = 90.0f, float fDefaultPos = 0.0f)
    {
        fJointMin = fMin;
        fJointMax = fMax;
        fPosition = 0.0f;
        SetTarget(fDefaultPos);
    }

    void SetTarget(float fAngle)
    {
        fTarget = fAngle;
        if(fTarget < fJointMin) fTarget = fJointMin;
        if(fTarget > fJointMax) fTarget = fJointMax;
    }
    
    void UpdatePosition()
    {
        fPosition = fTarget;
    }
    
    float GetTarget()
    {
        return fTarget;
    }
    
    float GetDutyCycle()
    {
        float fDutyCycle = fPosition / (fJointMax - fJointMin);
        fDutyCycle = (fDutyCycle * fDutyRange) + fDutyMin + (fDutyRange * 0.5f);
        return fDutyCycle;
    }
};


int main() 
{
    // Servos (MG996R) operate on 20ms period, so set
    // PWM period for each pin
    pin26.period(0.02f);
    pin25.period(0.02f);
    pin24.period(0.02f);
    pin23.period(0.02f);
    pin22.period(0.02f);
    pin21.period(0.02f);
    
    Joint joint[6];
    
    joint[0].SetTarget(0.0f);
    joint[1].SetTarget(0.0f);
    joint[2].SetTarget(0.0f);
    joint[3].SetTarget(-25.0f);
    joint[4].SetTarget(-20.0f);
    joint[5].SetTarget(-15.0f);
    
    int nTargetJoint = 0;
    
    while(1)
    {
        // Read from UART
        if(uart.readable())
        {
            unsigned char c = (unsigned char)uart.getc();
             
            if(c < 10)
               nTargetJoint = c;
            else
               joint[nTargetJoint].SetTarget((float)c - 128);   
               
        }
        
        
        // Write Duty Cycles
        // Update each joints position
        for(int i=0; i<6; i++)
            joint[i].UpdatePosition();
            
        // Set PWM values for each joint
        pin26.write(joint[0].GetDutyCycle());   
        pin25.write(joint[1].GetDutyCycle());   
        pin24.write(joint[2].GetDutyCycle());   
        pin23.write(joint[3].GetDutyCycle());   
        pin22.write(joint[4].GetDutyCycle());           
        pin21.write(joint[5].GetDutyCycle());   
        
    }   
}

*/