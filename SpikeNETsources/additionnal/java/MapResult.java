import java.io.*;
import java.lang.*;

class MapDescRes extends Object
{
		String 	map;
		int 	x;
		int 	y;
		int 	offsetx;
		int 	offsety;
		boolean desactivated;
			
	MapDescRes( String mapName, int coordx, int coordy, int offsetx0, int offsety0)
	{
		map = mapName; 
		x = coordx;
		y = coordy; 
		offsetx = offsetx0;
		offsety = offsety0;
		desactivated = false;
	}
		
	int testPos( int posx, int posy, String mapName) 
	{
		if ((Math.abs(posx-x)<=offsetx) && (Math.abs(posy-y)<=offsety))
		{
			if (desactivated) return -1;
			desactivated = true;
			if (mapName == map) 	return 2;
			else					return 1;
		}
		else return 0;
		// 2  = at this position
		// 1  = at this position but wrong map
		// -1 = at this position but position desactivated
		// 0  = not at this position
	}
};


class MapResult extends Object 
{
	
	final int MAX_IMAGE	=	50;
	
	MapDescRes 		tabRes[];
	int		   		nbItems = 0;
	

	MapResult( String fileName ) throws FileNotFoundException, IOException
	{
		InputStream			tmp_stream		= new FileInputStream(fileName);
		DataInputStream	file_desc_input 	= new DataInputStream(tmp_stream);
		
		while (file_desc_input.available() != 0) {

			String s = file_desc_input.readUTF();
			System.out.println( s );

		}
		
		file_desc_input.close();
	}

    public static void main(String[] arguments)  throws FileNotFoundException, IOException
    {      
        MapResult  app = new MapResult("salut");
    }

		/* writing mode 
		OutputStream		tmp_stream			= new FileOutputStream(fileName);
		DataOutputStream	file_desc_output 	= new DataOutputStream(tmp_stream);
		arch.writeFileInArchive("file2", file_desc_output, true);
		file_desc_output.close();*/
		
		/* reading mode */
		//arch.readArchive( fileNameArchive, new String("im"));
		

/*		FILESTREAM ff( fileName);
		char mapName[30], coordx[30], coordy[30], offsetx[30], offsety[30];
		nbItems = 0;
		int eof1 = EOF+1;
		do
		{
			ff >> mapName;
			ff >> coordx;
			ff >> coordy;
			ff >> offsetx;
			ff >> offsety;
			eof1 = ff.get();
			if ((eof1 != EOF) || (mapName[0] != 0))
				tabRes[nbItems++] = new MapDescRes( mapName, atoi(coordx), atoi(coordy), atoi(offsetx), atoi(offsety));
		} while (eof1 != EOF);
	}

	MapResult( char *fileName, char *ImageFileName, int offsetX = 0, int offsetY = 0)
	{
		pict = new PicturePPM( ImageFileName );
	
		FILESTREAM ff( fileName);
		char mapName[30], coordx[30], coordy[30], offsetx[30], offsety[30];
		nbItems = 0;
		int eof1 = EOF+1;
		do
		{
			ff >> mapName;
			ff >> coordx;
			ff >> coordy;
			ff >> offsetx;
			ff >> offsety;
			eof1 = ff.get();
			if ((eof1 != EOF) || (mapName[0] != 0))
				tabRes[nbItems++] = new MapDescRes( mapName, atoi(coordx)-offsetX, atoi(coordy)-offsetY, atoi(offsetx), atoi(offsety));
		} while (eof1 != EOF);
	}
	
	int testForRes( int posx, int posy, char *mapName)
	{
		int currentPos = 0;
		int res;
		while( (!(res = tabRes[ currentPos ]->testPos( posx, posy, mapName))) && (currentPos < nbItems)) currentPos++;
		
		if (currentPos != nbItems)
		{
			if (res == 2) pict->drawRect( tabRes[ currentPos ]->x-10, tabRes[ currentPos ]->y-13, tabRes[ currentPos ]->x+10, tabRes[ currentPos ]->y+13, 255);
			if (res == 1) pict->drawRect( tabRes[ currentPos ]->x-10, tabRes[ currentPos ]->y-13, tabRes[ currentPos ]->x+10, tabRes[ currentPos ]->y+13, 128);
			if (res == 2) printf("GOOD detection %s at poisition %d %d\n", mapName, posx, posy);
			else	if (res == 1) printf("BAD  detection %s at poisition %d %d\n", mapName, posx, posy);
					//else	if (res == -1) printf("ALREADY detected for %s at poisition %d %d\n", mapName, posx, posy);
		}
		else { res = 0; } //printf("no detection for map %s at position %d-%d\n", mapName, posx, posy); }
		return res;
	}
	void save( char *newFileName)
	{ pict->write_ppm( newFileName);}*/
};

