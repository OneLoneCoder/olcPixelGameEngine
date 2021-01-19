/*
	Controlling Elegoo Robot Smart Car Kit 3.0+ with ASIO and C++
	"Well, it better not leak anymore... stupid window" - javidx9

	Video: https://youtu.be/nkCP95zLvSQ	

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2020 OneLoneCoder.com

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

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	            https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020

*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define ASIO_STANDALONE
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <asio.hpp>

class RobotCar : public olc::PixelGameEngine
{
public:
	RobotCar()
	{		
		sAppName = "Robot Car - Simple Control";
	}

	asio::io_context context;
	std::unique_ptr<asio::serial_port> port;
	char nIncomingByte;
	std::string sIncomingData;
	std::thread thrContext;

	struct sButton
	{
		olc::vi2d vPos;
		olc::vi2d vSize;
		std::string sText;

		bool clicked(const olc::vi2d& vMousePos)
		{
			return vMousePos.x >= vPos.x &&
				vMousePos.x < (vPos.x + vSize.x) &&
				vMousePos.y >= vPos.y &&
				vMousePos.y < (vPos.y + vSize.y);
		}

		void draw(olc::PixelGameEngine* pge)
		{
			pge->FillRect(vPos, vSize, olc::BLUE);
			pge->DrawRect(vPos, vSize, olc::WHITE);
			olc::vi2d vTextSize = pge->GetTextSizeProp(sText);
			pge->DrawStringProp(vPos + (vSize - vTextSize) / 2, sText, olc::WHITE);
		}
	};

	sButton btnForwards;
	sButton btnBackwards;
	sButton btnTurnLeft;
	sButton btnTurnRight;

public:
	bool OnUserCreate() override
	{
		// Create serial port object...
		port.reset(new asio::serial_port(context));
	
		asio::error_code ec;
		port->open("COM4", ec);

		if (ec)
		{
			// Port could not be opened, so abort
			std::cout << ec.message() << std::endl;
			return false;
		}

		// ...it should have opened via creation, but check
		if (port->is_open())
		{
			// Set the BAUD rate
			port->set_option(asio::serial_port::baud_rate(9600));

			// By default the option arguments configure to 8N1...
			port->set_option(asio::serial_port_base::character_size());
			port->set_option(asio::serial_port_base::stop_bits());
			port->set_option(asio::serial_port_base::parity());

			// ...and flow control off
			port->set_option(asio::serial_port_base::flow_control());

			AsyncReadFromPort();
			thrContext = std::thread([&](){ context.run(); });
		}

		// Create Buttons
		btnForwards.vPos = { 78, 10 };
		btnForwards.vSize = { 100, 30 };
		btnForwards.sText = "Forwards";

		btnTurnLeft.vPos = { 10, 60 };
		btnTurnLeft.vSize = { 100, 30 };
		btnTurnLeft.sText = "Turn Left";

		btnTurnRight.vPos = { 146, 60 };
		btnTurnRight.vSize = { 100, 30 };
		btnTurnRight.sText = "Turn Right";

		btnBackwards.vPos = { 78, 110 };
		btnBackwards.vSize = { 100, 30 };
		btnBackwards.sText = "Backwards";

		return true;
	}

	bool OnUserDestroy() override
	{
		if (port)
		{
			// Abort any ongoing communications
			port->cancel();

			// Close the port
			port->close();

			// Don't erase the port...
		}

		// Wait for thread to expire
		if (thrContext.joinable())
		{			
			thrContext.join();
		}

		// ...until now, as the context may still need it before it expires		
		port.reset();

		return true;
	}

	void AsyncReadFromPort()
	{
		port->async_read_some(asio::buffer(&nIncomingByte, 1), [this](std::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					sIncomingData.append(1, nIncomingByte);
					std::cout << nIncomingByte;
					AsyncReadFromPort();
				}				
			});
	}


	

	bool OnUserUpdate(float fElapsedTime) override
	{
		asio::error_code ec;

		// Check input
		if (GetMouse(0).bPressed)
		{
			if (btnForwards.clicked(GetMousePos()))
			{
				port->write_some(asio::buffer("f", 1), ec);
				if (ec) std::cout << "Error Moving Forwards\n";
			}

			if (btnBackwards.clicked(GetMousePos()))
			{
				port->write_some(asio::buffer("b", 1), ec);
				if (ec) std::cout << "Error Moving Backwards\n";
			}

			if (btnTurnLeft.clicked(GetMousePos()))
			{
				port->write_some(asio::buffer("l", 1), ec);
				if (ec) std::cout << "Error Turning Left\n";
			}

			if (btnTurnRight.clicked(GetMousePos()))
			{
				port->write_some(asio::buffer("r", 1), ec);
				if (ec) std::cout << "Error Turning Right\n";
			}			
		}

		if (GetMouse(0).bReleased)
		{
			port->write_some(asio::buffer("s", 1), ec);
			if (ec) std::cout << "Error Stopping!!\n";
		}


		Clear(olc::BLACK);
		btnForwards.draw(this);
		btnTurnLeft.draw(this);
		btnTurnRight.draw(this);
		btnBackwards.draw(this);
		return true;
	}
};

int main()
{
	RobotCar demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}
