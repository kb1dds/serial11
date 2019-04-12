/* Send a listing file to a PDP-11 
 * 10 January 2003
 * Michael Robinson 
 */

#include <string.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
   FILE *infile, *device;
   char line[80];
   unsigned pc, address, data, i;
   unsigned char output, outputh, outputl;
   unsigned out, start;

   if( argc != 3 && argc != 4 )
   {
      fprintf( stderr, "usage:\n serial11 file device [start]\n");
      return( -1 );
   }

   start = 0;
   if( argc == 4 )
     sscanf( argv[3], "%o", &start );

   if( ( infile = fopen( argv[1], "rt" ) ) == NULL )
   {
      fprintf( stderr, "serial11: \"%s\" file error\n", argv[1] );
      return( -1 );
   }    
   if( ( device = fopen( argv[2], "wb" ) ) == NULL )
   {
      perror( "serial11: Device error" );
      return( -1 );
   }

   pc = -1;
   fprintf( stderr, "*Make sure you have run\n  stty raw -F %s\n\n", argv[2]);
   fprintf( stderr, "* Load the following program in a safe place:\n");
   fprintf( stderr, "105737; TSTB @#177560\n");   
   fprintf( stderr, "177560\n");   
   fprintf( stderr, "100375; BPL .-3 \n");   
   fprintf( stderr, "113722; MOVB @#177562,(R2)+\n");   
   fprintf( stderr, "177562\n");   
   fprintf( stderr, "000772; BR .-6\n");
   fprintf( stderr, "* Set the program counter to start this program\n");

   while( fgets( line, 80, infile ) != NULL )
   {
      /* Ignore blank or comment lines */
      if( line[0] == ';' || line[0] == '\n' || 
          line[0] == '\r' || line[0] == '\0' )
         continue;

      /* Trim comments */
      for( i = 0; i < 80; i ++ )
      {
         if( line[i] == ';' )
         {
            line[i] = '\0';
            break;
         }
      }

      if( sscanf( line, "%o %o", &address, &data ) != 2 )
      {
         fprintf( stderr, "serial11: Syntax error at address = %o\n", pc );
         break;
      }
      if( address < start )
 	 continue;
   
      if( pc == -1 || address != (pc + 2) )
      {
         if( pc != -1 )
            fprintf( stderr, "Please do the following:\n* Halt the machine\n");
         fprintf( stderr, "* Deposit %o into R2.\n", address );
         fprintf( stderr, "* Hit CONT\n" );
         fprintf( stderr, "Press enter once this is done\n" );
         getchar();
         pc = address;
      }
      else
      {
         pc = address;
      }

      outputl = (0x00ff & data);
      fwrite( &outputl, 1, 1, device );
      fflush( device );
      outputh = (0xff00 & data) >> 8;
      fwrite( &outputh, 1, 1, device ); 
      fflush( device );
   }

   fclose( infile );
   fclose( device );
}
