//int old[2] = {0,0};
void render(int del);
void clear();

// pin numbers
int pins[8][8] = {
	{2, 3, 4, 5, 6, 7, 8, 9}, // X
	{10, 11, 12, A0, A1, A2, A3, A4} // Y
};
int cells[8][8]  = {
	{1, 0, 1, 0, 1, 1},
	{0, 1, 0, 0, 1, 0, 1},
	{1, 0, 1, 0, 1, 1},
	{},
	{},
	{},
	{},
	{}
};

String serialData;


void setup() {
	for (int index = 2; index <= 12; index++) {
		pinMode(index, OUTPUT);
	}
	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	pinMode(A4, OUTPUT);
	pinMode(A5, INPUT); // refresh ratio potentiometer

	Serial.begin(9600);
	Serial.setTimeout(10);

	//clear
	for (int i = 0; i < 8; i++) {
		digitalWrite(pins[0][i], HIGH);
	}
}

void serialEvent() {
	clear();
	serialData = Serial.readString();

	// first two characters are snake length
	// after that, 1 char for x, 1 char for y for each snake bit
	// LLxyxyxyxyxyxy
	String slstr = serialData;
	slstr.remove(2); // remove everything that isn't the two first digits
	int snakeLen = slstr.toInt(); // starts counting from 1, not 0
	for (int i = 0; i < snakeLen; i++) {
		String x = (String)serialData.charAt(2+i*2);
		String y = (String)serialData.charAt(3+i*2);
		cells[x.toInt()][y.toInt()] = 1;
	}
}

void loop() {
	//cells[2][6] = 1;
	//cells[2][2] = 1;
	render(int(analogRead(A5))/5);
	//delay(int(analogRead(A5))/5);
}

void reset() {
	for (int xl = 0; xl < 8; xl++) {
		digitalWrite(pins[0][xl], HIGH);
	}
}
void clear() {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			cells[x][y] = 0;
		}
	}
}

void render(int del) {

	for (int yl = 0; yl < 8; yl++) {
		// The following will happen for every "Y line (row)"
		reset();

		digitalWrite(pins[1][yl], HIGH);

		for (int xl = 0; xl < 8; xl++) {
			if (cells[xl][yl] == 1) {
				digitalWrite(pins[0][7-xl], LOW);
			} else {
				digitalWrite(pins[0][7-xl], HIGH);
			}

		}
		delay(del);
		digitalWrite(pins[1][yl], LOW);
	}
}
