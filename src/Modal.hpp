#pragma once
#include <string>

class Modal
{
public:
	Modal(const std::string& name)
	{
		m_Name = name;
	};
	void Open();
	void Close();
	virtual void Render() = 0;
	bool m_ShouldOpen = false;
	std::string m_Name;
};
