int glob[7];

void main(void){
	int x;

	x = 10;
	glob[2] = x;
	glob[1] = 1;
	glob[3] = 3;
		
	if (glob[2] - x){
		glob[3] = glob[2] + x;
	}	
	else
		glob[1] = x;

	while (x > 0){
		x = x - 3;
	}

	output(glob[3]);
	output(glob[1]);
	output(x);
	
}

