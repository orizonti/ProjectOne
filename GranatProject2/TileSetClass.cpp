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

void TileSetClass::CreateTileSetFromMap(QString MapFilePath)
{
		QFile XMLMapFile(MapFilePath);
		bool result = XMLMapFile.open(QIODevice::ReadOnly);

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
					
					int type_id = newElement.attribute("firstgid").toInt();
					QString MapTilePath = newElement.attribute("source");

					UploadTerrainElementData(MapTilePath, type_id);
				}

				currentNod = currentNod.nextSibling();
			}
		}

}


void TileSetClass::UploadTerrainElementData(QString PathXMLFile, int type_id)
{
							QFile XMLMapFile(GameDir + "/WORK_DIR/MAPS_TILED/" + PathXMLFile);
						    bool result = XMLMapFile.open(QIODevice::ReadOnly);

							TerrainTileElement* newTileElement = new TerrainTileElement;

							QDomDocument newDomDoc;
							result = newDomDoc.setContent(&XMLMapFile);

							QDomElement tileset = newDomDoc.elementsByTagName("tileset").at(0).toElement();
							QDomElement tileoffset = newDomDoc.elementsByTagName("tileoffset").at(0).toElement();
							QDomElement image = newDomDoc.elementsByTagName("image").at(0).toElement();

							QString name = tileset.attribute("name");


							newTileElement->size.setWidth(tileset.attribute("tilewidth").toInt()); 
							newTileElement->size.setHeight(tileset.attribute("tileheight").toInt());

							newTileElement->Size_By_Cell.setWidth(newTileElement->size.width()/512); 
							newTileElement->Size_By_Cell.setHeight(newTileElement->size.height()/256);

							newTileElement->offset.first = tileoffset.attribute("x").toInt(); 
							newTileElement->offset.second = tileoffset.attribute("y").toInt();


							QString imageSource = image.attribute("source");
							imageSource.remove(0, 2);
							imageSource = GameDir + "/WORK_DIR" + imageSource;


									sf::Texture* textureTerrain = new sf::Texture;
									sf::Sprite*  spriteTerrain = new sf::Sprite;

									result = textureTerrain->loadFromFile(imageSource.toStdString());
											 spriteTerrain->setTexture(*textureTerrain);

											 newTileElement->Texture = textureTerrain;
											 newTileElement->Sprite = spriteTerrain;
											 newTileElement->type_id = type_id;
											 newTileElement->Name = name;

											 UploadGridLines(PathXMLFile, newTileElement);
											 newTileElement->GetHeightMap();

										TerrainElementsByType.insert(type_id, newTileElement);
										TerrainElementsByName.insert(name, newTileElement);

}

void TileSetClass::UploadGridLines(QString PathXMLSprite,TerrainTileElement* element)
{
	QString FileName = PathXMLSprite.split(".").first();
	QString PathTSX = QString("%1.tsx").arg(FileName);
	QString PathXMLLines = PathTSX.replace(".tsx", ".svg");

							QFile XMLGridFile(PathTSX);
						    bool result = XMLGridFile.open(QIODevice::ReadOnly);

							TerrainTileElement* newTileElement = new TerrainTileElement;

							QDomDocument newDomDoc;
							result = newDomDoc.setContent(&XMLGridFile);

							QDomElement tileoffset = newDomDoc.elementsByTagName("tileoffset").at(0).toElement();

							newTileElement->offsetGrid.first = tileoffset.attribute("x").toInt(); 
							newTileElement->offsetGrid.second = tileoffset.attribute("y").toInt();
							XMLGridFile.close();

	GridShapeContainer* newGrid = new GridShapeContainer;
	newGrid->AddCurves(PathXMLLines);
	element->GridLines = newGrid;
}

void TerrainTileElement::GetHeightMap()
{
	//qDebug() << "GET HEIGHT MAP";
	GameCoord LineCoord;
	if (GridLines->CurvesVert.isEmpty())
		return;
	
	int height_grid = GridLines->CurvesHoriz.size() - 1;
	int width_grid  = GridLines->CurvesVert.size() - 1;
	//qDebug() << "SECTOR COUNT - " << width_grid*height_grid << "HEIGHT GRID - " << height_grid;

	int x_cathetus = 0;
	int y_cathetus = 0;
	QPair<int,int> LinePos;
	//QPointF StartGridPoint = GridLines->CurvesVert.first().NodePoints.first();
	QPointF StartGridPoint; StartGridPoint.setX(0); StartGridPoint.setY(this->offset.second - size.height() + 128);

						double Height;
						QPointF NodePoint;
						int line = 0;
						int node = 0;
						int side = 0;
						qDebug() << "========================================================";
						for (int sector = 0; sector < width_grid * height_grid; sector++)
						{
							HeightMap.append(QVector<double>());
							line = sector / height_grid;
							node = sector - line*height_grid;
							qDebug() << "          ADD SECTOR NUMBER - " << sector << "LINE - " << line << "NODE - " << node;

						//	for(int corner = 0; corner < 4; corner++)
						//		{

						//		side = corner / 2;
						//        NodePoint = GridLines->CurvesVert.at(line + side).NodePoints.at(node + side); // ERROR !!!!!
						//		NodePoint = NodePoint + StartGridPoint;
						//		LineCoord.SetCoordIsometric(node+side,line + side);

						//		LinePos = LineCoord.GetDecCoord();
						//		//qDebug() << "NODE POINT - " << NodePoint << "LINE  - " << line + side << "IsoPos - " << LineCoord.GetIsoCoord();
						//		x_cathetus = NodePoint.x() - LinePos.first;
						//		y_cathetus = NodePoint.y() - LinePos.second;
						//		Height = std::hypot(x_cathetus, y_cathetus);

						//		HeightMap.last().append(Height);
						//		qDebug() << "Height - " << Height << "Side - " << side << "node - " << node;
						//				Draw_Height_Node NewNode;
						//				NewNode.DrawHeight.setString(std::to_string((int)Height));
						//				NewNode.NodePoint = GridLines->CurvesVert.at(line + side).NodePoints.at(node+side);
						//				HeightMapToDraw.append(NewNode);

						//		}

							for (int side = 0; side < 2; side++)
							{

							for(int corner = 0; corner < 2; corner++)
								{

						        NodePoint = GridLines->CurvesVert.at(line + side).NodePoints.at(node + corner); // ERROR !!!!!
								NodePoint = NodePoint + StartGridPoint;
								LineCoord.SetCoordIsometric(node+corner,line + side);

								LinePos = LineCoord.GetDecCoord();
								//qDebug() << "NODE POINT - " << NodePoint << "LINE  - " << line + side << "IsoPos - " << LineCoord.GetIsoCoord();
								x_cathetus = NodePoint.x() - LinePos.first;
								y_cathetus = NodePoint.y() - LinePos.second;
								Height = std::hypot(x_cathetus, y_cathetus);

								HeightMap.last().append(Height);
								//qDebug() << "Height - " << Height << "Side - " << side + line << "node - " << node + corner ;
										Draw_Height_Node NewNode;
										NewNode.DrawHeight.setString(std::to_string((int)Height));
										NewNode.NodePoint = GridLines->CurvesVert.at(line + side).NodePoints.at(node+corner);
										HeightMapToDraw.append(NewNode);

								}

							}
						}
							Size_By_Cell.setWidth(width_grid); 
							Size_By_Cell.setHeight(height_grid);
//qDebug() << "          HEIGHT MAP SIZE - " << HeightMap.size() << "HEIGHT MAP TO DRAW - " << HeightMapToDraw.size() <<"TERR SIZE - " << width_grid << height_grid;
//	qDebug() << "========================";
//	int n = 0;
//for (QVector<double> Heights : HeightMap)
//{
//	qDebug() << "<<<<<<" << n++ << ">>>>>>";
//	qDebug() << Heights;
//}
//	qDebug() << "========================";

}
