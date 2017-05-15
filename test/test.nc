main()
{
	int i ;
	int j ;
	int n;
	int tmp;
	int flag;
	read n;
	if( n<2 )
	{
		print 0;
	}
	else 
	{
		if ( n == 2)
		{
			print 2;
		}
		else
		{
			print 2;
			for( i =3; i<=n ; i = i+2 )
			{
				flag = 0;
				for( j =2; j< i;j = j+1)
				{
					tmp = i/j*j ;
					if( i - tmp  == 0)
					{
						flag = 1;
					}
				}
				if ( flag == 0)
				{
					print i;
				}
			}
		}
	}
}