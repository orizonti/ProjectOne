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
		
		qDebug() << "   <<<<  UPLOAD TEST HEIGHT MAP  >>>>";
		this->UploadTestHeightsMap();

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


void TileSetClass::UploadTestHeightsMap()
{
	QFile FileRead("E:/WorkDir/Heights1.xml");
	FileRead.open(QFile::ReadOnly);

	QXmlStreamReader xmlDoc(&FileRead);
	QString Sector;


	QVector < QVector<double>> HeightMap;
	while (!xmlDoc.atEnd() && !xmlDoc.hasError())
	{

		QXmlStreamReader::TokenType token = xmlDoc.readNext();

		if (token == QXmlStreamReader::EndElement)
		{
			if (xmlDoc.name() == "SECTOR")
			{
				int Node = 0;
			//	qDebug() << "SECTOR SIZE - " << HeightMap.size();
			//	for (auto QuadeHeight : HeightMap)
			//	{
			//		Node++;
			//		qDebug()<<"NODE "<<Node << "H1 - " << QuadeHeight[0] 
			//								<< "H2 - " << QuadeHeight[1] 
			//								<< "H3 - " << QuadeHeight[2] 
			//								<< "H4 - " << QuadeHeight[3];
			//	}
				//qDebug() << "=======================================";
				HeightsMap.insert(Sector,HeightMap);
			}
			xmlDoc.readNext();
		}

		if (token == QXmlStreamReader::StartElement)
		{
			if (xmlDoc.name() == "SECTOR")
			{
				//qDebug() << "=======================================";
				QXmlStreamAttributes attrib = xmlDoc.attributes();
				//qDebug() << "SECTOR - " << attrib.value("NAME").toString();
				Sector = attrib.value("NAME").toString();
				HeightMap.clear();
				HeightMap.resize(0);
				xmlDoc.readNext();
			}
		}

		QXmlStreamAttributes attrib = xmlDoc.attributes();

		QVector<double> Node;
		if (!xmlDoc.name().toString().isEmpty())
		{
			//qDebug() << "APPEND - " << xmlDoc.name();
		Node.append(attrib.value("H1").toDouble());
		Node.append(attrib.value("H2").toDouble());
		Node.append(attrib.value("H3").toDouble());
		Node.append(attrib.value("H4").toDouble());
		HeightMap.append(Node);
  		}

	}
	//qDebug() << "NUMBER OF SECTOR - " << HeightsMap.size();
	//qDebug() << "SECTORS - " << HeightsMap.keys();

	FileRead.close();
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


											 qDebug() << "TERRAIN ELEMENT- " << name;
											 newTileElement->GetHeightMap();
											 newTileElement->HeightMapConverted.append(HeightsMap[name]);
											 qDebug() << "HEIGHT MAP SIZE - " << newTileElement->HeightMapConverted.size();

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
	GameCoord LineCoord;
	if (GridLines->CurvesVert.isEmpty())
		return;
	
	int height_grid = GridLines->CurvesHoriz.size() - 1;
	int width_grid  = GridLines->CurvesVert.size() - 1;

	int x_cathetus = 0;
	int y_cathetus = 0;
	QPair<int,int> LinePos;
	QPointF StartGridPoint; StartGridPoint.setX(0); StartGridPoint.setY(this->offset.second - size.height() + 128);

						double Height;
						QPointF NodePoint;
						int line = 0;
						int node = 0;
						int side = 0;
						for (int sector = 0; sector < width_grid * height_grid; sector++)
						{
							HeightMap.append(QVector<double>());
							line = sector / height_grid;
							node = sector - line*height_grid;


							for (int side = 0; side < 2; side++)
							{

							for(int corner = 0; corner < 2; corner++)
								{

						        NodePoint = GridLines->CurvesVert.at(line + side).NodePoints.at(node + corner); 
								NodePoint = NodePoint + StartGridPoint;
								LineCoord.SetCoordIsometric(node+corner,line + side);

								LinePos = LineCoord.GetDecCoord();
								x_cathetus = NodePoint.x() - LinePos.first;
								y_cathetus = NodePoint.y() - LinePos.second;
								Height = std::hypot(x_cathetus, y_cathetus);

								HeightMap.last().append(Height);
										Draw_Height_Node NewNode;
										NewNode.DrawHeight.setString(std::to_string((int)Height));
										NewNode.NodePoint = GridLines->CurvesVert.at(line + side).NodePoints.at(node+corner);
										HeightMapToDraw.append(NewNode);

								}
							}
						}
							Size_By_Cell.setWidth(width_grid); 
							Size_By_Cell.setHeight(height_grid);

}
