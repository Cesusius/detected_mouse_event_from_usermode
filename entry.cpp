#include <iostream>
#include <Windows.h>
#include <vector>

HHOOK mouse_hook{ 0 }; 
HWND window_handle{ 0 }; 
DWORD process_id{ 0 };

auto low_level_mouse_proc( int nCode, WPARAM wParam, LPARAM lParam ) -> LRESULT CALLBACK {
	if ( nCode >= 0 ) {
		MSLLHOOKSTRUCT* mouse_event = ( MSLLHOOKSTRUCT* )lParam;

		window_handle = WindowFromPoint( mouse_event->pt );

		GetWindowThreadProcessId( window_handle, &process_id );

		//We make sure that we only block MouseEvent calls for process which have the "Injected" Flag. 
		//Everytime you have a program that simulates a own mouse(uses mouse_event) the mouse_event call will have the flag "injected"
		if ( mouse_event->flags & LLMHF_INJECTED ) {
			switch ( wParam ) {
			case WM_MOUSEMOVE:
				printf( "[Process %i] MouseEvent Call WM_MOUSEMOVE has been detected and blocked\n", process_id );
				return 1;
				break;
			case WM_MOUSEWHEEL:
				printf( "[Process %i] MouseEvent Call WM_MOUSEWHEEL has been detected and blocked\n", process_id );
				return 1;
				break;
			case WM_LBUTTONDOWN:
				printf( "[Process %i] MouseEvent Call WM_LBUTTONDOWN has been detected and blocked\n", process_id );
				return 1;
				break;
			case WM_RBUTTONDOWN:
				printf( "[Process %i] MouseEvent Call WM_RBUTTONDOWN has been detected and blocked\n", process_id );
				return 1;
				break;
			default:
			{
			}
			}
		}
	}
	return CallNextHookEx( mouse_hook, nCode, wParam, lParam ); 
}

auto setup_mouse_hook( ) -> void {
	mouse_hook = SetWindowsHookEx( WH_MOUSE_LL, low_level_mouse_proc, NULL, 0 );
	if ( !mouse_hook ) {
		printf( "Failed to setup Mouse Hook!\n" );
		exit( 0 ); 
	}
}

auto unhook_mouse( ) -> void {
	if ( mouse_hook != nullptr ) {
		UnhookWindowsHookEx( mouse_hook ); 
		mouse_hook = nullptr;
	}
}

auto main( ) -> std::uint32_t {
	printf( "Starting to monitor\n" );

	setup_mouse_hook( ); 

	MSG msg;
	while ( GetMessage( &msg, NULL, 0, 0 ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	unhook_mouse( ); 

	return 0;
}