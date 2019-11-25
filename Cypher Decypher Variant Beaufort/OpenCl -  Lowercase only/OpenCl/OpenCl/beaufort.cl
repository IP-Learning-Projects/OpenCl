bool isAlpha(char letter) {
	return (letter >= 'a' && letter <= 'z');
}

char wrapAroundRight(char letter, int positions) {
	if (letter + positions > 'z') {
		return letter - ('z' - 'a' + 1) + positions;
	}
	return letter + positions;
}

char wrapAroundLeft(char letter, int positions) {
	if (letter - positions < 'a') {
		return letter + ('z' - 'a' + 1) - positions;
	}
	return letter - positions;
}

char beaufort_encrypt(char letter, char key)
{
	if (!isAlpha(letter))
		return letter;

	int x;

	x = wrapAroundLeft(letter, (key - 'a'));
	return x;
}

char beaufort_decrypt(char letter, char key)
{
	if (!isAlpha(letter))
		return letter;

	int x;
	x = wrapAroundRight(letter, (key - 'a'));

	return x;
}

char beaufort_mode(char letter, char key, char mode) {
	if (mode == 'e') {
		return beaufort_encrypt(letter, key);
	}
	else {
		return beaufort_decrypt(letter, key);
	}
}


__kernel void beaufort(__global const char *mode,
						__global const char *inputString,
						 __global const int *len,
						__global const char *key,
						 __global const int *lenKey,
						__global char *outputString) {

	int i = get_global_id(0);

	outputString[i] = beaufort_mode(inputString[i],key[i % lenKey[0]],mode[0]);
}