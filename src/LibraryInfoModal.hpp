#pragma once
#include <imgui.h>
#include <memory>
#include "Modal.hpp"
#include "Player.h"


class LibraryInfoModal : Modal
{
public:
	LibraryInfoModal(const std::string& name) : Modal(name){}
	void Open(std::shared_ptr<Library> library);
	void Render() override;
	std::shared_ptr<Library> m_Library;
};
