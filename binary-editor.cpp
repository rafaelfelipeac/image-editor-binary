#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct rgb
{
        int r, g, b;
} rgb;

typedef struct imagem
{
        int w, h;
        rgb **pixels;
} imagem;

void menu();
void aloca(rgb ***p , int h, int w);
void alocaInt(int **i, int tam);
void alocaRGB(rgb **a, int tam);
void PeB(imagem img);
void sepia(imagem img);
int abre(imagem img);
int grava(imagem img);
void resize(imagem img, int w, int h);
void cortar(imagem img);
void girar(imagem img);
void rotate(rgb *pS, rgb *pD, int row, int col);
void volta(rgb *pSource, rgb *pDestination, int voltas, imagem img, int *m, int *n);
void desenhar();
double lenght(int x, int y, int CX, int CY);

main()
{
      char c;

      do
	  {
      	menu();

      	printf("\nDeseja continuar? <s/n>: ");
      	scanf("%c", &c);
      	fflush(stdin);
     } while(c != 'n' && c != 'N');

      system("pause");
}

void menu()
{
	int op;
	imagem img;

	do
 	{
      	printf("\n1 - Preto e branco\n2 - Sepia\n3 - Cortar\n4 - Rotacionar\n5 - Redimensionar\n6 - Desenhar\n7 - Sair\n\n");
      	printf("Opcao: ");
      	scanf("%i", &op);
      	fflush(stdin);
    } while(op < 1 || op > 7);
    
    if(op >= 1 && op < 6)
    {
    	printf("\nDigite a altura e largura da imagem: ");
      	scanf("%i %i", &img.h, &img.w);
      	fflush(stdin);

      	img.pixels = NULL;
   		aloca(&img.pixels, img.h, img.w);
   		printf("\n");
	}

    switch(op)
    {
    	case 1:
    		 PeB(img);
    		 break;
   		case 2:
   			 sepia(img);
   			 break;
 		case 3:
 			 cortar(img);
		 	 break;
		case 4:
			 girar(img);
			 break;
	    case 5:
	    	 resize(img, img.w, img.h);
	    	 break;
    	case 6:
    		 desenhar();
    		 break;
  		 case 7:
  		 	break;
    }
}

void alocaRGB(rgb **a, int tam)
{
	if((*a=(rgb *)realloc(*a, tam*sizeof(rgb)))==NULL)
		exit(1);
}

void alocaInt(int **i, int tam)
{
	if((*i=(int *)realloc(*i, tam*sizeof(int)))==NULL)
		exit(1);
}

void aloca(rgb ***p, int h, int w)
{
     int i;
     *p = (rgb**)realloc(*p, h*sizeof(rgb*));
     if(*p==NULL)
     {
     	printf("Erro de alocacao. (altura)\n");
        exit(1);
     }
     for(i=0; i<h; i++)
     {
     	*(*p+i) = NULL;
     }
     for(i=0; i<h; i++)
     {
          *(*p+i) = (rgb*)malloc(w*sizeof(rgb));
          if(*(*p+i)==NULL)
          {
          	printf("Erro de alocacao. (largura - linha %i)", i);
            exit(1);
          }
     }
}

void sepia(imagem img)
{
	printf("\tSEPIA\n");

	int i, j, tot = 0;
	int p1, p2, p3, p4;
	int resr, resg, resb, res;
	char nomeArquivo[80];
	FILE *f;
	rgb *pixel = NULL;
	
	alocaRGB(&pixel, 1);

	do
	{
		printf("Coordenadas (x,y) do primeiro ponto: ");
		scanf("%i %i", &p1, &p2);
		fflush(stdin);

		if(p1 < 0 || p2 < 0 || p1 > img.h || p2 > img.w)
			printf("Valores invalidos. Digite novamente.\n");
	} while(p1 < 0 || p2 < 0 || p1 > img.h || p2 > img.w);

	do
	{
		printf("Coordenadas (x,y) do segundo ponto: ");
		scanf("%i %i", &p3, &p4);
		fflush(stdin);

		if(p3 < 0 || p4 < 0 || p3 > img.h || p4 > img.w || p3 <= p1 || p4 <= p2)
			printf("Valores invalidos. Digite novamente.\n");
	} while(p3 < 0 || p4 < 0 || p3 > img.h || p4 > img.w || p3 <= p1 || p4 <= p2);
	
	do
	{
		printf("Nome + extensao do arquivo que deseja editar: ");
		gets(nomeArquivo);
		fflush(stdin);
		
		f = fopen(nomeArquivo, "rb+");
	} while(f == NULL);
	
	for(i=p1; i<=p3; i++)
	{
		for(j=p2; j<=p4; j++)
		{
			fseek(f, ((i-1)*img.w+j)*sizeof(rgb)+8, 0);
			fread(pixel, sizeof(rgb), 1, f);
		
			resr = pixel->r * 0.393 + pixel->g * 0.769 + pixel->b * 0.189;
			resg = pixel->r * 0.349 + pixel->g * 0.686 + pixel->b * 0.168;
			resb = pixel->r * 0.272 + pixel->g * 0.534 + pixel->b * 0.131; 

			resr = resr > 255 ? 255 : resr;
			resg = resg > 255 ? 255 : resg;
			resb = resb > 255 ? 255 : resb;

			pixel->r = resr;
			pixel->g = resg;
			pixel->b = resb;
			
			fseek(f, ((i-1)*img.w+j)*sizeof(rgb)+8, 0);
			fwrite(pixel, sizeof(rgb), 1 , f);
		}
	}
	
	fclose(f);
	
	printf("Arquivo editado.\n");	
}

void PeB (imagem img)
{
	printf("\tPRETO E BRANCO\n");

	int i, j, res = 0, aux = 0;

	do
	{
		aux = abre(img);
	} while(aux != 1);

	for(i=0; i<img.h; i++)
	{
		for(j=0; j<img.w; j++)
		{
			res = ((*(img.pixels+i)+j)->r) * 0.2989 + ((*(img.pixels+i)+j)->g) * 0.5870 + ((*(img.pixels+i)+j)->b) * 0.1140;

			((*(img.pixels+i)+j)->r) = res;
			((*(img.pixels+i)+j)->g) = res;
			((*(img.pixels+i)+j)->b) = res;
		}
    }

    aux = 0;

	do
	{
		aux = grava(img);
	} while(aux != 1);
	
	printf("Arquivo criado.\n");
}

int abre(imagem img)
{
     FILE *f;
	 int i, j, tot = 0;
	 char nomeArquivo[80];

	 printf("Nome + extensao do arquivo que deseja abrir: ");
	 gets(nomeArquivo);
	 fflush(stdin);

     if((f = fopen(nomeArquivo, "rb")) != NULL)
     {
		for(i=0; i<img.h; i++)
     	{
     		for(j=0; j<img.w; j++)
     		{
     			fseek(f, ((tot)*sizeof(rgb))+8, 0);
			 	fread((*(img.pixels+i)+j), sizeof(rgb), 1 , f);
			 	tot++;
			}
		}
		fclose(f);		
		return 1;
 	 }
 	 else
 	 {
 	 	printf("Arquivo nao existe ou esta corrompido. Digite novamente.\n");
 	 	return -1;
 	 }
}

int grava(imagem img)
{
	int i, j;
	char nomeArquivo[80];
	FILE *f;

	printf("Nome + extensao do arquivo de saida (se existir, sera sobrescrito): ");
	gets(nomeArquivo);
	fflush(stdin);

	if((f = fopen(nomeArquivo, "wb"))!=NULL)
    {
        fwrite(&img.w, sizeof(int), 1, f);
        fwrite(&img.h, sizeof(int), 1, f);

        for(i=0; i<img.h; i++)
        {
        	for(j=0; j<img.w; j++)
        	{
        		fwrite((*(img.pixels+i)+j), sizeof(rgb), 1, f);
        	}
        }
		fclose(f);
		return 1;
    }
    else
    {
    	printf("Problema ao abrir/criar arquivo. Tente novamente.\n");
    	return -1;
    }
}

void resize(imagem img, int w, int h)
{
	printf("\tREDIMENSIONAR\n");

	int aux = 0;

	do
 	{
	 	aux = abre(img);
	} while(aux != 1);

	int h2, w2;

	do
	{
		printf("Digite a altura2 e largura2: ");
		scanf("%i %i", &h2, &w2);
		fflush(stdin);
	} while(h2 < 0 || w2 < 0);

	int *temp = NULL;
	alocaInt(&temp, h2*w2*3); 
	rgb *pixels = NULL;
	alocaRGB(&pixels, h*w);
	int x, y, index = 0;
	float x_ratio = ((float)(w-1))/w2;
	float y_ratio = ((float)(h-1))/h2;
	float ww, hh, blue, red, green;
	int offset = 0;
	int i, j;

	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			*(pixels + index) = *((*(img.pixels+i)+j));
			index++;
		}
	}

	index = 0;

	for(i=0; i<h2; i++)
	{
		for(j=0; j<w2; j++)
		{
			x = (int) (x_ratio * j);
			y = (int) (y_ratio * i);
			ww = (x_ratio * j) - x;
            hh = (y_ratio * i) - y;

            index = (y*w+x);

			rgb A, B, C, D;
            A = *(pixels+index);
            B = *(pixels+(index+1));
            C = *(pixels+(index+w));
            D = *(pixels+(index+w+1));

            // Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
            blue = ((A.b)*(1-ww)*(1-hh) + (B.b)*(ww)*(1-hh) + (C.b)*(hh)*(1-ww) + (D.b)*(ww*hh));

            // Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
            green = ((A.g)*(1-ww)*(1-hh) + (B.g)*(ww)*(1-hh) + (C.g)*(hh)*(1-ww) + (D.g)*(ww*hh));

            // Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
            red = ((A.r)*(1-ww)*(1-hh) + (B.r)*(ww)*(1-hh) + (C.r)*(hh)*(1-ww) + (D.r)*(ww*hh));

	       	*(temp+offset) = red;
			offset++;
	       	*(temp+offset) = green;
			offset++;
			*(temp+offset) = blue;
			offset++;
        }
	}

	index = 0;

	aloca(&img.pixels, h2, w2);

	for(i=0; i<h2; i++)
	{
		for(j=0; j<w2; j++)
		{
			((*(img.pixels+i)+j)->r) = *(temp+index);
			index++;
			((*(img.pixels+i)+j)->g) = *(temp+index);
			index++;
			((*(img.pixels+i)+j)->b) = *(temp+index);
			index++;
		}
	}

	img.h = h2;
	img.w = w2;

	grava(img);
	
	printf("Arquivo criado/editado.\n");
}

void cortar(imagem img)
{
	printf("\tCORTAR\n");

	int p1, p2, p3, p4;
	char nomeArquivo[80], nomeArquivo2[80];
	int i, j, tot = 0, aux = 0;
	FILE *f, *fn;
	rgb *pixel = NULL;
	int h2 = 0, w2 = 0;
	
	alocaRGB(&pixel, 1);

	do
	{
		printf("Digite as coordenadas (x,y) do primeiro ponto: ");
		scanf("%i %i", &p1, &p2);
		fflush(stdin);

		if(p1 < 0 || p1 > img.h || p2 < 0 || p2 > img.w)
			printf("Valores invalidos. Digite novamente.\n");

	} while(p1 < 0 || p1 > img.h || p2 < 0 || p2 > img.w);

	do
	{
		printf("Digite as coordenadas (x,y) do segundo ponto: ");
		scanf("%i %i", &p3, &p4);
		fflush(stdin);

		if(p3 < 0 || p3 > img.h || p4 < 0 || p4 > img.w || p3 <= p1 || p4 <= p2)
			printf("Valores invalidos. Digite novamente.\n");

	} while(p3 < 0 || p3 > img.h || p4 < 0 || p4 > img.w || p3 <= p1 || p4 <= p2);

	do
	{
		printf("Nome + extensao do arquivo que deseja abrir: ");
		gets(nomeArquivo);
		fflush(stdin);

		f = fopen(nomeArquivo, "rb");
	} while(f == NULL);
	
	do
	{
		printf("Nome + entensao do arquivo que deseja criar: ");
		gets(nomeArquivo2);
		fflush(stdin);
		
		fn = fopen(nomeArquivo2, "wb");
	} while(fn == NULL);
	
	h2 = p3-p1;
	w2 = p4-p2;
	
	fwrite(&w2, sizeof(int), 1, fn);
	fwrite(&h2, sizeof(int), 1, fn);
	
	for(i=p1; i<p3; i++)
	{
		for(j=p2; j<p4; j++)
		{
			fseek(f, ((i-1)*img.w+j)*sizeof(rgb)+8, 0);
			fread(pixel, sizeof(rgb), 1, f);
		
			fseek(fn, tot*sizeof(rgb)+8, 0);
			fwrite(pixel, sizeof(rgb), 1 , fn);
			
			tot++;
		}
	}
	
	fclose(f); 
	fclose(fn);
	
	printf("Arquivo criado.\n");
}

void girar(imagem img)
{
	printf("\tROTACIONAR\n");

	int i, j, k;
	FILE *f;
	rgb *pSource;
	rgb *pDestination;
	int m = img.h, n = img.w;
	int op = 0;
	int tot = 0;
	int graus = 0, voltas = 0;
	bool valido = false;

	do
	{
		op = abre(img);
	} while(op != 1);

	pSource = (rgb *)malloc(sizeof(rgb) * m * n);
	pDestination = (rgb *)malloc(sizeof(rgb) * m * n);

	for(i=0; i<img.h; i++)
	{
		for(j=0; j<img.w; j++)
		{
			*(pSource+tot) = *((*(img.pixels+i)+j));
			tot++;
		}
	}

	do
	{
		printf("Deseja girar a imagem em quantos graus? (multiplos de 90): ");
		scanf("%i", &graus);
		fflush(stdin);

		if(graus%90 == 0)
		{
			valido = true;
			break;
		}
		else
			printf("Valor invalido. Digite novamente.\n");
	} while(valido == false);

	if(graus > 0)
	{
		voltas = abs(graus) / 90;
		volta(pSource, pDestination, voltas, img, &m, &n);
	}
	if(graus < 0)
	{
		int valor = graus / 90;

		if(valor%2==0)
			voltas = abs(valor);
		else
		{
			if(valor != -1)
			{
				voltas = abs(abs(valor) - 2);
			}
			else
			{
				voltas = abs(valor - 2);
			}
		}

		volta(pSource, pDestination, voltas, img, &m, &n);
	}

	if(graus != 0)
	{
		img.h = m;
		img.w = n;
		aloca(&img.pixels, img.h, img.w);
		tot = 0;

		for(i=0; i<img.h; i++)
		{
			for(j=0; j<img.w; j++)
			{
				*((*(img.pixels+i)+j)) = *(pDestination+tot);
				tot++;
			}
		}
	}
	
	op = 0;

	do
	{
		op = grava(img);
	} while(op != 1);
	
	printf("Arquivo criado.\n");
}

void rotate(rgb *pS, rgb *pD, int row, int col)
{
    int r, c, rr, gg, bb;

	for (r = 0; r < row; r++)
    {
        for (c = 0; c < col; c++)
        {
            *(pD + c * row + (row - r - 1)) = *(pS + r * col + c);
        }
    }
}

void volta(rgb *pSource, rgb *pDestination, int voltas, imagem img, int *m, int *n) 
{
	int k, i, j, tot = 0;

	for(k=1; k<=voltas; k++)
	{
		int mm = *m, nn = *n;
		tot = 0;

		rotate(pSource, pDestination, mm, nn);

		for(i=0; i<mm; i++)
		{
			for(j=0; j<nn; j++)
			{
				*(pSource+tot) = *(pDestination+tot);
				tot++;
			}
		}

		if(k%2 != 0)
		{
			*m = img.w;
			*n = img.h;
		}
		else
		{
			*m = img.h;
			*n = img.w;
		}
	}
}

void desenhar()
{
	printf("\tDESENHAR\n");
	
	FILE *f;
	char nomeArquivo[80], op;
	int h, w, hf, wf, lt, x, y, tot = 0, i, j, limX, limY, tb = 0, contd = 0, conte = 0, raio = 0, n, r = 0, g = 0, b = 0;
	rgb *pixel = NULL;

	alocaRGB(&pixel, 1);
	pixel->r = 255;
	pixel->g = 255;
	pixel->b = 255;

	printf("Digite o nome da imagem + extencao: ");
	gets(nomeArquivo);
	fflush(stdin);

	f = fopen(nomeArquivo, "wb");

	printf("Altura e largura da imagem: ");
	scanf("%i %i", &h, &w);
	fflush(stdin);

	fwrite(&h, sizeof(int), 1, f);
	fwrite(&w, sizeof(int), 1, f);

	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			fseek(f, tot*sizeof(rgb)+8, 0);
			fwrite(pixel, sizeof(rgb), 1, f);
			tot++;
		}
	}

	do
	{
		do
		{
			printf("1 - Quadrado\n2 - Triangulo\n3 - Circulo\n\nOpcao: ");
			scanf("%i", &n);
			fflush(stdin);
		} while(n <1 || n >3);

		if(n == 1)
		{
			tot = 0;
			printf("Ponto (x,y) do inicio do quadrado: ");
			scanf("%i %i", &x, &y);
			fflush(stdin);

			printf("Altura e largura do quadrado: ");
			scanf("%i %i", &hf, &wf);
			fflush(stdin);

			printf("Tamanho da borda: ");
			scanf("%i", &tb);
			fflush(stdin);
			
			printf("Cor do quadrado (RGB): \n");
			printf("0 - 255\n");
			
			do
			{
				printf("R: ");
				scanf("%i", &r);
				fflush(stdin);
			} while(r < 0 || r > 255); 
			do
			{
				printf("G: ");
				scanf("%i", &g);
				fflush(stdin);
			} while(g < 0 || g > 255); 
			do
			{
				printf("B: ");
				scanf("%i", &b);
				fflush(stdin);
			} while(b < 0 || b > 255); 

			pixel->r = r;
			pixel->g = g;
			pixel->b = b;

			limX = tb*0.9;
			limY = tb*0.9;

			for(i=0; i<h; i++)
			{
				for(j=0; j<w; j++)
				{
					if((i>=x && i<=(x+hf)) && (j>=y && j<=(y+wf)))
					{
						if(!((i>(x+limX) && i<(x+hf-limX)) && (j>(y+limY) && j<(y+wf-limY))))
						{
							fseek(f, tot*sizeof(rgb)+8,0);
							fwrite(pixel, sizeof(rgb), 1, f);
						}
					}
					tot++;
				}
			}
		}
		
		if(n == 2)
		{
			tot = 0;
			printf("Ponto (x,y) do topo do triangulo: ");
			scanf("%i %i", &x, &y);
			fflush(stdin);

			printf("Lado do triangulo: ");
			scanf("%i", &lt);
			fflush(stdin);

			printf("Tamanho da borda: ");
			scanf("%i", &tb);
			fflush(stdin);
			
			printf("Cor do quadrado (RGB): \n");
			printf("0 - 255\n");
			
			do
			{
				printf("R: ");
				scanf("%i", &r);
				fflush(stdin);
			} while(r < 0 || r > 255); 
			do
			{
				printf("G: ");
				scanf("%i", &g);
				fflush(stdin);
			} while(g < 0 || g > 255); 
			do
			{
				printf("B: ");
				scanf("%i", &b);
				fflush(stdin);
			} while(b < 0 || b > 255); 

			pixel->r = r;
			pixel->g = g;
			pixel->b = b;
			
			conte = 0;
			contd = 0;

			limX = tb;
			limY = tb;


			for(i=0; i<h; i++)
			{
                for(j=0; j<w; j++)
                {	
					if(!((i>(x+limX) && i<(x+lt-(limX*0.7))) && (j<(y+contd-limY) && j>(y-conte+limY)))) 
					{	
						if(i == x+contd && j == y+contd && contd <= lt)
	                    {
	                   		fseek(f, tot*sizeof(rgb)+8, 0);
				 			fwrite(pixel, sizeof(rgb), 1, f);
				 			contd++;
	                   	}
	                    if(i == x+conte && j == y-conte && conte <= lt)
	   					{
						 	fseek(f, tot*sizeof(rgb)+8, 0);
					 		fwrite(pixel, sizeof(rgb), 1, f);
					 		conte++;
						}
						if((i >= x && i <= x+lt) && (i>= x+contd && j>=y-conte) && (j >= y-lt && j <= y+lt)) 
						{
						 	fseek(f, tot*sizeof(rgb)+8, 0);
						 	fwrite(pixel, sizeof(rgb), 1, f);
						}
					}
					tot++;
                }
            }
		}
		
		if(n == 3)
		{
			tot = 0;
			
			printf("Ponto (x,y) do centro do circulo: ");
			scanf("%i %i", &x, &y);
			fflush(stdin);
			
			printf("Raio do circulo: ");
			scanf("%i", &raio);
			fflush(stdin);
			
			printf("Tamanho da borda: ");
			scanf("%i", &tb);
			fflush(stdin);
			
			printf("Cor do quadrado (RGB): \n");
			printf("0 - 255\n");
			
			do
			{
				printf("R: ");
				scanf("%i", &r);
				fflush(stdin);
			} while(r < 0 || r > 255); 
			do
			{
				printf("G: ");
				scanf("%i", &g);
				fflush(stdin);
			} while(g < 0 || g > 255); 
			do
			{
				printf("B: ");
				scanf("%i", &b);
				fflush(stdin);
			} while(b < 0 || b > 255); 

			pixel->r = r;
			pixel->g = g;
			pixel->b = b;
			
			for(i=0; i<h; i++)
			{
				for(j=0; j<w; j++)
				{
					if(lenght(i, j, x, y) < raio && lenght(i, j, x, y) > (raio-(tb*0.9)))
					{
						fseek(f, tot*sizeof(rgb)+8, 0);
						fwrite(pixel, sizeof(rgb), 1, f);
					}
					tot++;
				}
			}
		}

		printf("Deseja desenhar mais uma figura? <S/N>: ");
		scanf("%c", &op);
		fflush(stdin);
	} while(op == 's' || op == 'S');

	fclose(f);
	
	printf("Arquivo criado.\n");
}

double lenght(int x, int y, int CX, int CY) 
{
    return sqrt(((x - CX)*(x - CX)) + ((y - CY)*(y - CY))); 
}
