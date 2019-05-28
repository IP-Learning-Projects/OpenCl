
bool isLowerCase(char letter) {
	return (letter >= 'a' && letter <= 'z');
}

bool isUpperCase(char letter) {
	return (letter >= 'A' && letter <= 'Z');
}

bool isAlpha(char letter) {
	return isLowerCase(letter) || isUpperCase(letter);
}

char toLowerCase(char letter) {
	if (isLowerCase(letter)) {
		return letter;
	}
	return (letter - 'A') % ('z' - 'a' + 1) + 'a';
}

char toUpperCase(char letter) {
	if (isUpperCase(letter)) {
		return letter;
	}
	return (letter - 'a') % ('z' - 'a' + 1) + 'A';
}

char wrapAroundRight(char letter, int positions) {
	if (isLowerCase(letter)) {
		if (letter + positions > 'z') {
			return letter - ('z' - 'a' + 1) + positions;
		}
	}
	else {
		if (letter + positions > 'Z') {
			return letter - ('z' - 'a' + 1) + positions;
		}
	}
	return letter + positions;
}
char wrapAroundLeft(char letter, int positions) {
	if (isLowerCase(letter)) {
		if (letter - positions < 'a') {
			return letter + ('z' - 'a' + 1) - positions;
		}
	}
	else {
		if (letter - positions < 'A') {
			return letter + ('z' - 'a' + 1) - positions;
		}
	}
	return letter - positions;
}

char vigenere_encrypt(char letter, char key)
{
	if (!isAlpha(letter))
		return letter;

	int x;

	if (isLowerCase(letter)) {
		if (isUpperCase(key)) {
			key = toLowerCase(key);
		}
		x = wrapAroundRight(letter, (key - 'a'));
	}
	else {
		if (isLowerCase(key)) {
			key = toUpperCase(key);
		}
		x = wrapAroundRight(letter, (key - 'A'));
	}
	return x;
}

char vigenere_decrypt(char letter, char key)
{

	if (!isAlpha(letter))
		return letter;

	int x;

	if (isLowerCase(letter)) {
		if (isUpperCase(key)) {
			key = toLowerCase(key);
		}
		x = wrapAroundLeft(letter, (key - 'a'));
	}
	else {
		if (isLowerCase(key)) {
			key = toUpperCase(key);
		}
		x = wrapAroundLeft(letter, (key - 'A'));
	}

	return x;
}

char vigeneree(char letter, char key, char mode) {
	if (mode == 'e' || mode == 'E') {
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