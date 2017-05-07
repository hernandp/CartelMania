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

	size_t GetCount() const
	{
		return m_table.size();
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
