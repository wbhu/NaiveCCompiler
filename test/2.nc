main()
{
	int i;
	int n;
	int j;
	j = 1;
	read n;
	if(n > 5)
	{
		for(i = 1;i < n;i = i+ 1)
		{
			j = j * i;
		}
	}
	else
	{
		while(n>=0)
		{
			print n;
		}
	}
}