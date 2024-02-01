#pragma once

class VFTFunctionData
{
public:
	VFTFunctionData(void* fn, size_t fn_idx) : m_function(fn), m_functionIndex(fn_idx)
	{
	}

	auto get_function()
	{
		return reinterpret_cast<uintptr_t>(m_function);
	}

	auto get_function_index()
	{
		return m_functionIndex + 1;
	}

	void* m_function{};
	size_t m_functionIndex{};
};

class VFT
{
public:
	VFT(VFTFunctionData* vft, size_t vft_size) : m_vft(vft), m_vftSize(vft_size)
	{
	}

	VFT(std::vector<VFTFunctionData> vft) : m_vft(vft.data()), m_vftSize(vft.size())
	{
	}

	VFT(void* vft, size_t vft_size) : m_vft(static_cast<VFTFunctionData*>(vft)), m_vftSize(vft_size)
	{
	}

	auto get_vft()
	{
		return m_vft;
	}

	auto get_vft_as_uint()
	{
		return reinterpret_cast<uintptr_t**>(get_vft());
	}

	auto get_vft_size()
	{
		return m_vftSize;
	}

	VFTFunctionData* m_vft{};
	size_t m_vftSize{};
};

class hookVFT
{
public:
	hookVFT(VFT vft, VFT replacement_vft) :
		m_vft(vft.get_vft_as_uint()), m_vftSize(vft.get_vft_size()),
		m_copy_of_orginial_vft(*m_vft), m_table_of_replace_functions(std::make_unique<uintptr_t[]>(m_vftSize + 1))
	{
		std::copy_n(m_copy_of_orginial_vft - 1, m_vftSize + 1, m_table_of_replace_functions.get());
		for (size_t i{}; i != replacement_vft.get_vft_size(); ++i)
		{
			hook(replacement_vft.get_vft()[i]);
		}
	}

	void enable()
	{
		*m_vft = &m_table_of_replace_functions[1];
	}

	void disable()
	{
		for (size_t i{}; i != m_vftSize; ++i)
		{
			if (m_table_of_replace_functions[i + 1] != m_copy_of_orginial_vft[i])
			{
				m_table_of_replace_functions[i + 1] = m_copy_of_orginial_vft[i];
			}
		}
		*m_vft = m_copy_of_orginial_vft;
	}

	void hook(VFTFunctionData data)
	{
		m_table_of_replace_functions[data.get_function_index()] = data.get_function();
	}

	void unhook(size_t index)
	{
		m_table_of_replace_functions[index + 1] = m_copy_of_orginial_vft[index];
	}

	template <typename Type>
	Type get_original(size_t index)
	{
		return Type(m_copy_of_orginial_vft[index]);
	}

	uintptr_t** m_vft;
	size_t m_vftSize;

	uintptr_t* m_copy_of_orginial_vft;
	std::unique_ptr<uintptr_t[]> m_table_of_replace_functions;
};
