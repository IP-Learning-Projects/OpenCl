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

char vigenere_encrypt(char letter, char key)
{
	if (!isAlpha(letter))
		return letter;

	int x;

	x = wrapAroundRight(letter, (key - 'a'));
	return x;
}

char vigenere_decrypt(char letter, char key)
{
	if (!isAlpha(letter))
		return letter;

	int x;
	x = wrapAroundLeft(letter, (key - 'a'));

	return x;
}

char vigeneree(char letter, char key, char mode) {
	if (mode == 'e') {
		return vigenere_encrypt(letter, key);
	}
	else {
		return vigenere_decrypt(letter, key);
	}
}


__kernel void vigenere(__global const char *mode,
						__global const char *inputString,
						 __global const int *len,
						__global const char *key,
						 __global const int *lenKey,
						__global char *outputString) {

	int i = get_global_id(0);

	outputString[i] = vigeneree(inputString[i],key[i % lenKey[0]],mode[0]);
}