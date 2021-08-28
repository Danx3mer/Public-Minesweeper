#include "Miner.h"

HWND hwnd; //Parent window handle.

/// <summary>
/// The starting point of the program.
/// </summary>
/// <param name="argc: ">Size of the array of nullterminated strings.</param>
/// <param name="argv: ">Pointer to an array of nullterminated strings.</param>
/// <returns>A value of 0.</returns>
int main(int argc, char** argv)
{
	const char* HWNDTitle = "MINESWEEPER"; //Window Title.

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(Window_W, Window_H);

	glutCreateWindow(HWNDTitle);

	glutSetWindowTitle(HWNDTitle);

	hwnd = FindWindowA(0, HWNDTitle);
	if (hwnd == NULL)
		exit(0x3); //Exit (code 3) if there is no Handle to the parent window found.

	glutDisplayFunc(display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutTimerFunc(0, Timer, 0);
	glutKeyboardFunc(Keyboard);

	init();

	glutMainLoop();

	return 0;
}
