#pragma once

namespace lua100::win
{
  struct dll final
  {
    inline explicit operator HMODULE(void) noexcept { return m_handle; }
    inline explicit operator HMODULE(void) const noexcept { return m_handle; }

    inline bool operator!(void) const noexcept { return m_handle == static_cast<HMODULE>(NULL); }

    inline bool operator==(const HMODULE _value) const noexcept { return m_handle == _value; }

    dll& operator= (dll&& _value) noexcept
    {
      m_handle = std::exchange<HMODULE, HMODULE>(_value.m_handle, NULL);
      return *this;
    }

    dll(void) noexcept :
      m_handle{ NULL }
    {}

    dll(dll&& _value) noexcept :
      m_handle{ std::exchange<HMODULE, HMODULE>(_value.m_handle, NULL) }
    {}

    dll(const std::filesystem::path& _value) noexcept :
      m_handle{ LoadLibrary(_value.c_str()) }
    {}

    dll(const std::filesystem::directory_entry& _value) noexcept :
      m_handle{ LoadLibrary(_value.path().c_str()) }
    {}

    ~dll(void)
    {
      if (NULL != m_handle) {
        FreeLibrary(m_handle);
      }
    }

  private:
    HMODULE m_handle{ NULL };
  };
}