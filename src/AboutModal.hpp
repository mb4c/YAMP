#pragma once

#include "Modal.hpp"
#include "Texture.hpp"

class AboutModal : Modal
{
public:
	AboutModal(const std::string& name) : Modal(name)
	{}

	void Open();

	void Render() override;
private:
	Texture m_Logo;
};
