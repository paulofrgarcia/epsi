finput x:
	void:void;
	int x;


terminal t:
	void:out;

finput solo:
	void:x;
	int x;

foutput y:

	void:solo;
	int x:x.x+1;
	int y:solo.x;

finput in:
	void:y;
	int x;

foutput out:
	void:in;
	int z:1;

