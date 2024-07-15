#pragma once

#include "Modal.hpp"

class AboutModal : Modal
{
public:
	AboutModal(const std::string& name) : Modal(name)
	{}

	void Open();

	void Render() override;
};
