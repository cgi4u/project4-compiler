int glob[7];

void main(void){
	int x;

	x = 5;
	glob[2] = x;
	
	if (glob[2] + x){
		glob[3] = glob[2] + x;
	}	
	else
		glob[1] = x;

	while (x){
		x = x - x;
	}

	output(glob[3]);
	output(x);
}

