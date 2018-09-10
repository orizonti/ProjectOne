#include "TileSetClass.h"


TerrainTileElement::TerrainTileElement()
{

}
TerrainTileElement::~TerrainTileElement()
{
	delete Texture;
	delete Sprite;
}


TileSetClass::TileSetClass()
{
}


TileSetClass::~TileSetClass()
{

	for (TerrainTileElement* Tile : TerrainElementsByType)
	{
		delete Tile;
	}
}

//FINDING TILESET ELEMENTS AND CREATE TILE ELEMENT OBJECT TO EACH OF THEM
void TileSetClass::CreateTileSetFromMap(QString MapFilePath)
{
		QFile XMLMapFile(MapFilePath);                      //MAIN TMX FILE WITH MAP DATA
		bool result = XMLMapFile.open(QIODevice::ReadOnly);
		
		QFile XMLwithHeights("E:/WorkDir/Heights1.xml");
		XMLwithHeights.open(QFile::ReadOnly);

//=========================================================================================================
		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&XMLMapFile);
		if (result)
		{
			QDomNode currentNod = newDomDoc.documentElement().firstChild();
			while (!currentNod.isNull()) 
			{
				QDomElement newElement = currentNod.toElement();

				if (newElement.tagName() == "tileset")
				{
					
					int type_id = newElement.attribute("firstgid").toInt(); // NUMBER OF TILE ELEMENT IN MAIN MAP FILE
					QString MapTilePath = newElement.attribute("source");   // PATH TO TSX FILE THAT CONTAINS TILE DATA SUCH OFFSET, SIZE , NAME and PATH TO PNG IMAGE

					TerrainTileElement* newTileElement = new TerrainTileElement;
					newTileElement->UploadData(MapTilePath, type_id);
					newTileElement->UploadHeightMap(XMLwithHeights);
					newTileElement->GetHeightMapToDraw(); // SHOUD BE DELETED !!!!
					XMLwithHeights.seek(0);

										TerrainElementsByType.insert(type_id, newTileElement);
										TerrainElementsByName.insert(newTileElement->Name, newTileElement);
				}

				currentNod = currentNod.nextSibling(); 
			}
		}
//=========================================================================================================

	XMLwithHeights.close();
	XMLMapFile.close();
}


// UPLOADS SIZE, NAME, TEXTURE, SVG GRID LINES TO TILE ELEMENT
void TerrainTileElement::UploadData(QString PathXMLFile, int element_number)
{

							QFile XMLMapFile(GameDir + "/WORK_DIR/MAPS_TILED/" + PathXMLFile);
						    XMLMapFile.open(QIODevice::ReadOnly);


							//=======================================================
							QDomDocument newDomDoc;
							newDomDoc.setContent(&XMLMapFile);

							QDomElement tileset    = newDomDoc.elementsByTagName("tileset").at(0).toElement();
							QDomElement tileoffset = newDomDoc.elementsByTagName("tileoffset").at(0).toElement();
							QDomElement image      = newDomDoc.elementsByTagName("image").at(0).toElement();

							//=======================================================
		                    type_id = element_number;
							Name = tileset.attribute("name");
							Size_In_Pixels.setWidth(tileset.attribute("tilewidth").toInt()); 
							Size_In_Pixels.setHeight(tileset.attribute("tileheight").toInt());

							Size_By_Cell.setWidth(Size_In_Pixels.width()/512); 
							Size_By_Cell.setHeight(Size_In_Pixels.height()/256);

							offset.first = tileoffset.attribute("x").toInt(); 
							offset.second = tileoffset.attribute("y").toInt();
							//=======================================================


							//=======================================================
							QString imageSource = image.attribute("source");
							imageSource.remove(0, 2);
							imageSource = GameDir + "/WORK_DIR" + imageSource;


									sf::Texture* textureTerrain = new sf::Texture;
									sf::Sprite*  spriteTerrain = new sf::Sprite;

									         textureTerrain->loadFromFile(imageSource.toStdString());
											 spriteTerrain->setTexture(*textureTerrain);

											 Texture = textureTerrain;
											 Sprite = spriteTerrain;
							//=======================================================

											 UploadGridData(PathXMLFile);

}

// UPLOADS SVG GRIDS, THAT IS USED TO DISPLAY GRIDS AND DEFINCE CELL PRESSING ON CURVED CELL
void TerrainTileElement::UploadGridData(QString PathXMLFile)
{
	QString FileName = PathXMLFile.split(".").first();      //GET FILE NAME FROM FILE_NAME.tmx
	QString PathTSX = QString("G%1.tsx").arg(FileName);     //GRID OFFSET FROM TILED MAP EDITOR
	QString PathXMLLines = QString("%1.svg").arg(FileName); //SVG CURVES FROM PHOTOSHOP
	//=======================================================

	//=================================================================
							QFile XMLGridFile(PathTSX);             //GET GRID OFFSET
						    XMLGridFile.open(QIODevice::ReadOnly);

							QDomDocument newDomDoc;
							newDomDoc.setContent(&XMLGridFile);

							QDomElement tileoffset = newDomDoc.elementsByTagName("tileoffset").at(0).toElement();

							offsetGrid.first = tileoffset.attribute("x").toInt(); 
							offsetGrid.second = tileoffset.attribute("y").toInt();
							XMLGridFile.close();
	//=================================================================

	GridLines = new GridShapeContainer;
	GridLines->AddCurves(PathXMLLines);  //LOAD CURVES FROM SVG FILE

	Size_By_Cell.setWidth(GridLines->CurvesVert.size() - 1);   //IF PLAIN TILE IT IS GETTING FROM TILE SIZE BY PIXELS in UploadData 
	Size_By_Cell.setHeight(GridLines->CurvesHoriz.size() - 1); //IF CURVED HILL TILE SIZE IS ETTING FROM GRID LINE COUNTS
}

//UPLOAD HILL HEIGHTS THAT WAS GETTING FROM PNG HEIGHTS FILE, CREATED IN PHOTOSHOP
//PNG FILE WAS CONVERETED TO SVG TABLE IN OTHER UTILITY PROGRAM
void TerrainTileElement::UploadHeightMap(QFile& XMLFileHeights)
{
	QXmlStreamReader xmlDoc(&XMLFileHeights);
	QString Sector;

	//============================================================
	while (!xmlDoc.atEnd() && !xmlDoc.hasError())
	{   // SEEK XML ELEMENT BY NAME OF TERRAIN ELEMENT
		xmlDoc.readNextStartElement();
	    Sector = xmlDoc.attributes().value("NAME").toString();
		if (Sector == Name)
			break;
	}
	//============================================================

	//IF ELEMENT HAVE BENN FOUND BY NAME THEN READ ITS HEIGHTS TABLE
	QXmlStreamAttributes attrib;
	QXmlStreamReader::TokenType token;
	while (!xmlDoc.atEnd() && !xmlDoc.hasError())
	{

		token = xmlDoc.readNext();
		if (token == QXmlStreamReader::EndElement)
		{
			if (xmlDoc.name() == "SECTOR") // END ELEMENT WITH NAME SECTOR IT IS END OF HEIGHTS TABLE TO CURRENT ELEMENT
				return;
			xmlDoc.readNext();
		}


		attrib = xmlDoc.attributes();

		QVector<double> Node;
		if (!xmlDoc.name().toString().isEmpty())
		{
		Node.append(attrib.value("H1").toDouble());  //GET FOUR HEIGHTS TO ON CELL NODE
		Node.append(attrib.value("H2").toDouble());
		Node.append(attrib.value("H3").toDouble());
		Node.append(attrib.value("H4").toDouble());
		HeightMap.append(Node);
  		}

	}
}




void TerrainTileElement::GetHeightMapToDraw()
{


						int line = 0;
						int node = 0;
						int side = 0;
						for (int sector = 0; sector < HeightMap.size(); sector++)
						{
							line = sector / Size_By_Cell.height();
							node = sector - line*Size_By_Cell.height();

							for (int side = 0; side < 2; side++)
							{

							for(int corner = 0; corner < 2; corner++)
								{

										Draw_Height_Node NewNode;
										NewNode.DrawHeight.setString(std::to_string((int)HeightMap[sector][side*2 + corner]));
										NewNode.NodePoint = GridLines->CurvesVert.at(line + side).NodePoints.at(node+corner);

										HeightMapToDraw.append(NewNode);
								}
							}
						}
}
