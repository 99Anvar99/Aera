#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"

namespace util
{
	class clipboard
	{
	public:
		clipboard(std::string value) : m_buffer(value)
		{
		}

		clipboard()
		{
		}

		bool get()
		{
			if (open())
			{
				if (getData())
				{
					m_buffer = static_cast<char*>(lock());
					unlock();
					close();
					return true;
				}
				close();
			}
			return false;
		}

		bool set()
		{
			if (m_buffer.empty())
				return false;
			if (open() && empty())
			{
				//why the fuck am I missing a character? because MicroDick is retarded.
				// Why should the developer account for an extra 2 characters because of their shit code?
				// What the fuck...
				if (allocate(m_buffer.size() + 1))
				{
					memcpy(lock(), m_buffer.c_str(), m_buffer.size());
					unlock();
					setData();
					close();
					free();
					return true;
				}
				close();
			}
			return false;
		}

		clipboard operator=(clipboard clipboard)
		{
			clipboard.set();
			return clipboard;
		}

		std::string& str()
		{
			return m_buffer;
		}

	private:
		bool getData()
		{
			m_handle = GetClipboardData(CF_TEXT);
			return m_handle;
		}

		bool setData()
		{
			return SetClipboardData(CF_TEXT, m_handle);
		}

		bool allocate(size_t size)
		{
			m_handle = GlobalAlloc(GMEM_MOVEABLE, size);
			return m_handle;
		}

		bool free()
		{
			return GlobalFree(m_handle);
		}

		void* lock()
		{
			return GlobalLock(m_handle);
		}

		bool unlock()
		{
			return GlobalUnlock(m_handle);
		}

		bool open()
		{
			return OpenClipboard(nullptr);
		}

		bool empty()
		{
			return EmptyClipboard();
		}

		bool close()
		{
			return CloseClipboard();
		}

	private:
		std::string m_buffer{};
		HGLOBAL m_handle{};
	};
}
