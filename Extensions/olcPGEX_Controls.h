/*
	olcPGEX_Controls.h
	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                       Controls                              |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	some basic controls and event handlers.

	Example
	~~~~~~~

	#define OLC_PGE_APPLICATION
	#include "olcPixelGameEngine.h"

	#define OLC_PGEX_CONTROLS
	#include "olcPGEX_Controls.h"

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	class ControlsTest : public olc::PixelGameEngine
	{
	public:
		ControlsTest()
		{
		}
	private:
		olc::ctrls::Slider red;
		olc::ctrls::Slider green;
		olc::ctrls::Slider blue;
	public:

		bool OnUserCreate() override
		{

			olc::ctrls::Initialize(this);

			olc::ctrls::Slider red(olc::vf2d(200, 200), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->red = red;

			olc::ctrls::Slider green(olc::vf2d(200, 250), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->green = green;

			olc::ctrls::Slider blue(olc::vf2d(200, 300), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->blue = blue;

			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			Clear(olc::BLACK);

			this->red.Update();
			this->green.Update();
			this->blue.Update();

			FillRect(690, 190, 110, 120, olc::Pixel(r, g, b));
			DrawRect(690, 190, 110, 120, olc::WHITE);

			r = this->red.Value(255);
			g = this->green.Value(255);
			b = this->blue.Value(255);
			return true;
		}
	};

	int main()
	{
		ControlsTest demo;
		if (demo.Construct(1100, 500, 1, 1))
			demo.Start();
		return 0;
	}

	What's new in 09.05.20
	~~~~~~~~~~~~~~~~~~~~~~
	  - Even more names are understandable
	  - Added the 'AnyEvent' event
	  - Other stuff.
*/

#ifdef OLC_PGEX_CONTROLS
#undef OLC_PGEX_CONTROLS

namespace olc
{
	/* olcPixelGameEngine Controls extension functions & classes */
	namespace ctrls
	{
		const enum Orientation
		{
			VERTICAL, HORIZONTAL, AUTO
		};

		olc::PixelGameEngine* pge;

		void Initialize(olc::PixelGameEngine* pixelGameEngine)
		{
			pge = pixelGameEngine;
		}

		struct Box
		{
			int x;
			int y;
			int width;
			int height;
			Box(int x, int y, int width, int height);
			bool Contains(int pointX, int pointY);
		};

		Box::Box(int x, int y, int width, int height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		bool Box::Contains(int pointX, int pointY)
		{
			if (pointX > x && pointX <= x + width && pointY >= y && pointY <= y + height)
			{
				return true;
			}
			return false;
		}

		class BaseControl;
		class EventHandler
		{
		public:
			virtual void MouseClicked(olc::PixelGameEngine* pge, BaseControl* control) {}
			virtual void MouseReleased(olc::PixelGameEngine* pge, BaseControl* control) {}
			virtual void MouseHover(olc::PixelGameEngine* pge, BaseControl* control) {}
			virtual void ValueChanged(olc::PixelGameEngine* pge, BaseControl* control, float newValue) {}
			virtual void AnyEvent(olc::PixelGameEngine* pge, BaseControl* control) {}
		};

		class BaseControl
		{
		public:
			std::vector<EventHandler*> eventHandlers;
			bool lockHandlers = false;
			bool lockUpdates = false;
			void LockHandlers()
			{
				lockHandlers = true;
			}
			void FreeHandlers()
			{
				lockHandlers = false;
			}
			bool HandlersLocked()
			{
				return lockHandlers;
			}
			void LockUpdates()
			{
				lockUpdates = true;
			}
			void FreeUpdates()
			{
				lockUpdates = false;
			}
			bool UpdatesLocked()
			{
				return lockUpdates;
			}
			virtual float GetX()
			{
				return 0.0f;
			}

			virtual float GetY()
			{
				return 0.0f;
			}

			virtual float GetWidth()
			{
				return 0.0f;
			}

			virtual float GetHeight()
			{
				return 0.0f;
			}
			virtual void UpdateSelf() {}
			void Update()
			{
				if (!lockUpdates)
					UpdateSelf();
			}
			void AddEventHandler(EventHandler* evh)
			{
				this->eventHandlers.push_back(evh);
			}
			void CallMouseClicked(BaseControl* ctrl)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->MouseClicked(pge, ctrl);
						handler->AnyEvent(pge, ctrl);
					}
			}
			void CallMouseReleased(BaseControl* ctrl)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->MouseReleased(pge, ctrl);
						handler->AnyEvent(pge, ctrl);
					}
			}
			void CallMouseHover(BaseControl* ctrl)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->MouseHover(pge, ctrl);
						handler->AnyEvent(pge, ctrl);
					}
			}
			void CallValueChanged(BaseControl* ctrl, float newValue)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->ValueChanged(pge, ctrl, newValue);
						handler->AnyEvent(pge, ctrl);
					}
			}
		};

		/*
		--------------------------------------------------
				All controls are registered below
		--------------------------------------------------
		*/

		class Button : public BaseControl
		{
		public:
			float x;
			float y;
			float width;
			float height;
			float textXOffset;
			float textYOffset;
			olc::Pixel backgroundColor;
			olc::Pixel backgroundColorHover;
			olc::Pixel foregroundColor;
			olc::Pixel foregroundColorHover;
			std::string text;
			int32_t textScale;
			Button(olc::vf2d position = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), std::string text = "Button", int32_t textScale = 2U, olc::Pixel backgroundColor = olc::Pixel(44, 62, 80), olc::Pixel backgroundColorHover = olc::Pixel(52, 73, 94), olc::Pixel foregroundColor = olc::Pixel(236, 240, 241), olc::Pixel foregroundColorHover = olc::Pixel(236, 240, 241))
			{
				this->x = position.x;
				this->y = position.y;
				this->width = size.x;
				this->height = size.y;
				this->backgroundColor = backgroundColor;
				this->backgroundColorHover = backgroundColorHover;
				this->foregroundColor = foregroundColor;
				this->foregroundColorHover = foregroundColorHover;
				this->text = text;
				this->textXOffset = 0;
				this->textYOffset = 0;
				this->textScale = textScale;
			}
			void PlaceText(float tpx, float tpy)
			{
				textXOffset = tpx;
				textYOffset = tpy;
			}
			float GetX() override
			{
				return x;
			}
			float GetY() override
			{
				return y;
			}
			float GetWidth() override
			{
				return width;
			}
			float GetHeight() override
			{
				return height;
			}
			float GetTextXOffset()
			{
				return this->textXOffset;
			}
			void UpdateSelf() override
			{
				olc::Pixel backgroundColor = this->backgroundColor;
				olc::Pixel foregroundColor = this->foregroundColor;

				Box* boundingBox = new Box(x, y, width, height);

				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY())) {
					CallMouseHover(this);
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						CallMouseClicked(this);
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						CallMouseReleased(this);

					backgroundColor = backgroundColorHover;
					foregroundColor = foregroundColorHover;
				}
				pge->FillRect(x, y, width, height, backgroundColor);
				pge->DrawString(x + textXOffset, y + textYOffset, text, foregroundColor, textScale);
				delete boundingBox;
			}
		};

		class ProgressBar : public BaseControl
		{
		public:
			float x;
			float y;
			float width;
			float height;
			float value;
			float targetValue = 0.0f;
			olc::Pixel backgroundColor;
			olc::Pixel foregroundColor;
			Orientation orientation;
			ProgressBar(olc::vf2d position = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), Orientation orientation = Orientation::HORIZONTAL, olc::Pixel backgroundColor = olc::Pixel(88, 92, 92), olc::Pixel foregroundColor = olc::Pixel(39, 174, 96))
			{
				this->x = position.x;
				this->y = position.y;
				this->width = size.x;
				this->height = size.y;
				this->value = 0.0f;
				this->backgroundColor = backgroundColor;
				this->foregroundColor = foregroundColor;

				if (orientation == AUTO)
					if (width > height)
						orientation = HORIZONTAL;
					else if (width < height)
						orientation = VERTICAL;
					else
						orientation = HORIZONTAL;

				this->orientation = orientation;
			}
			float GetX() override
			{
				return x;
			}
			float GetY() override
			{
				return y;
			}
			float GetWidth() override
			{
				return width;
			}
			float GetHeight() override
			{
				return height;
			}
			/* Gets the progress size of horizontal progressbar */
			float GetProgressWidth()
			{
				return (width / 100) * value;
			}
			/* Gets the progress size of vertical progressbar */
			float GetProgressHeight()
			{
				return (width / 100) * value;
			}
			Orientation GetOrientation()
			{
				return orientation;
			}
			void Increment(float incrementValue)
			{
				targetValue = value + incrementValue;
				CallValueChanged(this, value);
			}
			/* Only Increments if value in range */
			void SafeIncrement(float incrementValue)
			{
				if (!IsFull())
					Increment(incrementValue);
			}
			void Decrement(float decrementValue) {
				targetValue = value - decrementValue;
				CallValueChanged(this, value);
			}
			/* Only Decrements if value in range */
			void SafeDecrement(float decrementValue)
			{
				if (!IsEmpty())
					Decrement(decrementValue);
			}
			void SetValue(float newValue)
			{
				if (newValue != value)
				{
					CallValueChanged(this, value);
				}
				targetValue = newValue;
			}
			float GetValue()
			{
				return value;
			}
			bool IsFull()
			{
				return value >= 100;
			}
			bool IsEmpty()
			{
				return value <= 0;
			}
			void UpdateSelf() override
			{
				Box* boundingBox = new Box(x, y, width, height);
				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
				{
					CallMouseHover(this);
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						CallMouseClicked(this);
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						CallMouseReleased(this);
				}

				// Smooth progress bars
				if ((int)value != (int)targetValue)
				{
					if ((int)targetValue < (int)value)
					{
						if (!IsEmpty())
							value -= 0.4f;
					}
					else if ((int)targetValue > (int)value)
					{
						if (!IsFull())
							value += 0.4f;
					}
				}
				float newWidth = width;
				float newHeight = height;
				switch (orientation)
				{
				case HORIZONTAL:
					newWidth = (width / 100) * value;
					break;
				case VERTICAL:
					newHeight = (height / 100) * value;
					break;
				}
				pge->FillRect(x, y, width, height, backgroundColor);
				pge->FillRect(x, y, newWidth, newHeight, foregroundColor);
				delete boundingBox;
			}
		};

		class Slider : public BaseControl
		{
		public:
			float x;
			float y;
			float size;
			float headOffset;
			olc::Pixel backgroundColor;
			olc::Pixel foregroundColor;
			Orientation orientation;
			bool IsSelected = false;
			Slider(olc::vf2d position = olc::vf2d(0.0f, 0.0f), float size = 0.0f, Orientation orientation = AUTO, olc::Pixel backgroundColor = olc::Pixel(88, 92, 92), olc::Pixel foregroundColor = olc::Pixel(39, 174, 96))
			{
				this->x = position.x;
				this->y = position.y;
				this->size = size;
				this->headOffset = 0;
				this->backgroundColor = backgroundColor;
				this->foregroundColor = foregroundColor;

				if (orientation == AUTO)
					orientation = HORIZONTAL;

				this->orientation = orientation;
			}
			float GetX() override
			{
				return x;
			}
			float GetY() override
			{
				return y;
			}
			float GetWidth() override
			{
				return size;
			}
			float GetHeight() override
			{
				return size;
			}
			void SetHeadOffset(float hoffset)
			{
				headOffset = hoffset;
			}
			float GetHeadOffset()
			{
				return headOffset;
			}
			float GetPercent()
			{
				return headOffset / (size / 100);
			}
			float Value(float maxv)
			{
				return (maxv / 100) * GetPercent();
			}
			void UpdateSelf() override
			{
				if (orientation == HORIZONTAL)
				{
					Box* boundingBox = new Box(x + headOffset, y - 30 / 2, 10, 30);
					if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
					{
						CallMouseHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							IsSelected = true;
							CallMouseClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							CallMouseReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						IsSelected = false;
					}
					if (IsSelected)
					{
						int newOffset = pge->GetMouseX() - x;
						if (newOffset >= -1 && newOffset <= (int)size + 1)
						{
							headOffset = newOffset;
							CallValueChanged(this, headOffset);
						}
					}
					pge->FillRect(x, y, size, 5, backgroundColor);
					pge->FillRect(x + headOffset, y - 30 / 2, 10, 30, foregroundColor);
					delete boundingBox;
					return;
				}

				if (orientation == VERTICAL)
				{
					Box* boundingBox = new Box(x - 30 / 2, y + headOffset, 30, 10);
					if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
					{
						CallMouseHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							IsSelected = true;
							CallMouseClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							CallMouseReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						IsSelected = false;
					}
					if (IsSelected)
					{
						int newOffset = pge->GetMouseY() - y;
						if (newOffset >= -1 && newOffset <= (int)size + 1)
						{
							headOffset = newOffset;
							CallValueChanged(this, headOffset);
						}
					}
					pge->FillRect(x, y, 5, size, backgroundColor);
					pge->FillRect(x - 30 / 2, y + headOffset, 30, 10, foregroundColor);
					delete boundingBox;
					return;
				}
				return;
			}
		};

		class CheckBox : public BaseControl
		{
		public:
			float x;
			float y;
			float textXOffset = 0;
			float textYOffset = 0;
			wint_t textScale;
			std::string text;
			olc::Pixel outlineColor;
			olc::Pixel selectionColor;
			olc::Pixel textColor;
			bool Checked = false;
			CheckBox(olc::vf2d position = olc::vf2d(0, 0), std::string text = "CheckBox", wint_t textScale = 2, olc::Pixel outlineColor = olc::Pixel(236, 240, 241), olc::Pixel selectionColor = olc::Pixel(231, 76, 60), olc::Pixel textColor = olc::Pixel(236, 240, 241))
			{
				this->x = position.x;
				this->y = position.y;
				this->text = text;
				this->outlineColor = outlineColor;
				this->selectionColor = selectionColor;
				this->textColor = textColor;
				this->textScale = textScale;
			}
			void PlaceText(float textXOffset, float textYOffset) {
				this->textXOffset = textXOffset;
				this->textYOffset = textYOffset;
			}
			float GetX() override
			{
				return x;
			}
			float GetY() override
			{
				return y;
			}
			bool IsChecked() {
				return Checked;
			}
			void Toggle()
			{
				Checked = !Checked;
			}
			void UpdateSelf() override
			{
				Box* boundingBox = new Box(x, y, 20, 20);
				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY())) {
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed) {
						Checked = !Checked;
						float newValue;
						switch (Checked) {
						case true:
							newValue = 1;
							break;
						case false:
							newValue = 0;
							break;
						}
						CallMouseClicked(this);
						CallValueChanged(this, newValue);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased) {
						CallMouseReleased(this);
					}
				}

				if (Checked)
					pge->DrawString(x + 3, y + 3, "X", selectionColor, 2);
				pge->DrawRect(x, y, 20, 20, outlineColor);
				pge->DrawString(x + textXOffset, y + textYOffset, text, textColor, textScale);
				delete boundingBox;
			}
		};

		/* Similar to a slider but in a circle */
		class Wheel : public BaseControl
		{
		public:
			olc::Pixel outlineColor;
			olc::Pixel dotColor;
			olc::Pixel backgroundColor;
			int x;
			int y;
			int width;
			int height;
			int dotSize;
			float angle = 0.0f;
			float radius = 20;
			int32_t lastValue = 0;
			Wheel(olc::vf2d position = olc::vf2d(0, 0), float radius = 0.0f, float dotSize = 0.0f, olc::Pixel outlineColor = olc::Pixel(236, 240, 241), olc::Pixel dotColor = olc::Pixel(46, 204, 113), olc::Pixel bgColor = olc::Pixel(0, 0, 0))
			{
				this->outlineColor = outlineColor;
				this->dotColor = dotColor;
				this->backgroundColor = backgroundColor;
				this->radius = radius;
				this->x = position.x;
				this->y = position.y;
				this->dotSize = dotSize;
			}
			float GetX() override
			{
				return x;
			}
			float GetY() override
			{
				return y;
			}
			float GetPercent()
			{
				return 100 * (angle / 133.0f) + 0.9;
			}
			float Value(float maxv)
			{
				return (maxv / 100) * GetPercent();
			}
			float GetAngle()
			{
				return angle;
			}
			void SetAngle(float angle)
			{
				angle = angle;
			}
			const float MaxAngle()
			{
				return 133.0f;
			}
			void UpdateSelf() override
			{
				Box* boundingBox = new Box(x - radius, y - radius, radius * 2, radius * 2);
				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
				{
					CallMouseHover(this);
					if (lastValue != pge->GetMouseWheel())
					{
						CallValueChanged(this, GetPercent());
					}
					lastValue = pge->GetMouseWheel();
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
					{
						CallMouseClicked(this);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						CallMouseReleased(this);
					}
					angle += (float)pge->GetMouseWheel() / 20;
				}
				if ((int)GetPercent() > 100)
					angle = 0;
				if (angle < 0)
					angle = 0;
				pge->FillCircle(x, y, radius, backgroundColor);
				pge->DrawCircle(x, y, radius, outlineColor);
				pge->FillCircle(x + radius / 2 * sin(angle), y + radius / 2 * cos(angle), dotSize, dotColor);
				delete boundingBox;
			}
		};


		/* A basic checkbox pool */
		class CheckBoxPool : public EventHandler
		{
		public:
			std::vector<CheckBox*> checkboxVector;
			CheckBoxPool(std::vector<CheckBox*> checkboxVector = {})
			{
				this->checkboxVector = checkboxVector;
			}
			void AddItem(CheckBox* checkbox)
			{
				this->checkboxVector.push_back(checkbox);
			}
			void SetItems(std::vector<CheckBox*> checkboxVector)
			{
				this->checkboxVector = checkboxVector;
			}
			void ValueChanged(olc::PixelGameEngine* pge, BaseControl* ctrl, float nVal) override
			{
				for (CheckBox* cb : checkboxVector)
				{
					cb->Checked = false;
				}
				((CheckBox*)ctrl)->Checked = true;
			}
		};

		/* The easiest way of setting up a CPool_Handler */
		CheckBoxPool* SetupPool(std::vector<CheckBox*> checkboxVector)
		{
			CheckBoxPool* checkBoxPool = new CheckBoxPool();
			checkBoxPool->checkboxVector = checkboxVector;
			for (CheckBox* cb : checkboxVector)
			{
				cb->AddEventHandler(checkBoxPool);
			}
			return checkBoxPool;
		}

	}
}
#endif
