#pragma once

namespace Steins
{
	class Texture
	{
	public :
		virtual ~Texture() = default;

		virtual UInt32 GetWidth() const = 0;
		virtual UInt32 GetHeight() const = 0;

		virtual void Bind() const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static Shared<Texture2D> Create();
	};
}