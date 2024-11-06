#pragma once

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <string>

namespace swarm::test
{
    class MemoryLeak
    {
    private:
        _CrtMemState mem_state_old;
        _CrtMemState mem_state_new;
        _CrtMemState mem_state_diff;

    public:
        const wchar_t* name;

        MemoryLeak(const wchar_t* const default_name):
            name { default_name },
            mem_state_old { },
            mem_state_new { },
            mem_state_diff { }
        {
        }

        void checkpoint()
        {
            _CrtMemCheckpoint(&mem_state_old);
        }

        void compare()
        {
            _CrtMemCheckpoint(&mem_state_new);
            std::wstring message { L"----------- " + std::wstring { name } + L" memory state:" };
            if (_CrtMemDifference(&mem_state_diff, &mem_state_old, &mem_state_new))
            {
                OutputDebugString(message.c_str());
                OutputDebugString(L"\n-----------_CrtMemDumpStatistics ---------");
                _CrtMemDumpStatistics(&mem_state_diff);
                OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
                _CrtMemDumpAllObjectsSince(&mem_state_old);
                OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
                _CrtDumpMemoryLeaks();
            }
            else
            {
                OutputDebugString((message + L" No leaks are detected :) ---------\n").c_str());
            }
        }
    };
}

#endif

