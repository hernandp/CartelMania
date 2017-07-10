/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
#pragma once
#include <vector>

template <class Derived_T, class ObjectT>
class NamedTable
{
public:
	NamedTable()
	{
		static_cast<Derived_T*>(this)->Init();
	}

	const ObjectT& LookupName(const std::wstring& name)
	{
		auto ret = std::find_if(m_table.cbegin(), m_table.cend(),
			[&name](const std::pair<std::wstring, ObjectT>& entry)
		{
			return entry.first == name;
		}
		);

		if (ret == m_table.end())
		{
			throw std::runtime_error("Lookup failed, object name not found in named table");
		}

		return ret->second;
	}

	int GetCount() const
	{
		return (int) m_table.size();
	}

	const auto&  At(int index)
	{
		return m_table.at(index);
	}

	const std::wstring& NameAt(int index)
	{
		return At(index).first;
	}

	const ObjectT& ObjectAt(int index)
	{
		return At(index).second;
	}

protected:
	std::vector <std::pair<std::wstring, ObjectT> > m_table;
};
