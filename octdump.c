/* Read an octal dump file */

#include <stdio.h>

int main( int argc, char *argv[] )
{
   FILE *fp;
   unsigned char data1, data2;
   unsigned data;

   if( argc != 2 )
   {
      fprintf(stderr, "usage:\n octdump file\n" );
      return( -1 );
   }

   if( (fp = fopen( argv[1], "rb" ) ) == NULL )
   {
      fprintf(stderr,"octdump: %s file error\n", argv[1] );
      return( -1 );
   }

   while( !feof( fp ) )
   {
      fread( &data1, 1, 1, fp );
      fread( &data2, 1, 1, fp );
      data = (data2 << 8) | data1; 
      printf( "%6o\n", data );
   } 

   fclose( fp );
}
