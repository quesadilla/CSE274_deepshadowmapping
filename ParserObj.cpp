#include "ParserObj.h"


ParserObj::ParserObj()
{
	vertexPositions = std::vector<Vector3>();
	vertexColors = std::vector<Vector3>();
	vertexNormals = std::vector<Vector3>();
	facePositions = std::vector<Vector3>();
	faceNormals = std::vector<Vector3>();

	objScale = 7.5;
	objScale2 = 1;
	S = S.makeScale(objScale, objScale, objScale);
	S2 = S2.makeScale(objScale2, objScale2, objScale2);
	model2world = Matrix4::identity() * S;
	angleZ = 0.0;
	angleY = 0.0;
	angleX = 0.0;
	spinVelocity = 1;

	arbitAngle = 0.0;
	axis = Vector3(1,1,1);
}

void ParserObj::update()
{
	S = S.makeScale(objScale, objScale, objScale);
	S2 = S2.makeScale(objScale2, objScale2, objScale2);
	model2world = Matrix4::identity() * Matrix4::makeRotateX(angleX) \
		* Matrix4::makeRotateY(angleY) * Matrix4::makeRotateZ(angleZ);
	model2world = model2world * S2 * S;
	model2world = model2world * Matrix4::makeRotate(arbitAngle, axis);
}

void ParserObj::draw(Light * rotLight, Matrix4 * glmatrix)
{
	// establish camera angle
	//Vector3 e = Vector3(0, 0, -10);
	//Vector3 d = Vector3(0, 0, 0);
	//Vector3 up = Vector3(0, 1, 0);

	// camera.recalculateMatrix(e, d, up);

	rotLight->bunnyMaterial();

	rotLight->display();


	// load the camera
	glmatrix = &getMatrix();
	glmatrix = &(glmatrix->transpose()); // not sure if I'm supposed to transpose this
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(glmatrix->getPointer());

	glEnable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);

	int indexPos = -1;
	int indexNorm = -1;

	float colorR = 0;
	float colorG = 0;
	float colorB = 0;
	float vertexX = 0;
	float vertexY = 0;
	float vertexZ = 0;
	float normX = 0;
	float normY = 0;
	float normZ = 0;

	int size = getFaceSize();

	// loops over every triangle
	for (int i = 0; i < size; i++)
	{
		// for each vertex, get the required indices for positions and normals
		// then draw them

		// FIRST vertex
		// get required indices
		indexPos = (getFacePos(i).getX()) - 1; // faces start at 1 but first index should be 0
		indexNorm = (getFaceNorm(i).getX()) - 1; // same as above

		vertexX = getVertex(indexPos).getX(); // set vertex x y z positions
		vertexY = getVertex(indexPos).getY();
		vertexZ = getVertex(indexPos).getZ();

		colorR = getVertColor(indexPos).getX(); // set vertex r g b color
		colorG = getVertColor(indexPos).getY();
		colorB = getVertColor(indexPos).getZ();

		normX = getVertNorm(indexNorm).getX(); // set vertex normal x y z positions
		normY = getVertNorm(indexNorm).getY();
		normZ = getVertNorm(indexNorm).getZ();

		glNormal3f(normX, normY, normZ);
		glVertex3f(vertexX, vertexY, vertexZ);
		glColor3f(colorR, colorG, colorB);

		// SECOND vertex
		// get required indices
		indexPos = (getFacePos(i).getY()) - 1; // faces start at 1 but first index should be 0
		indexNorm = (getFaceNorm(i).getY()) - 1; // same as above

		vertexX = getVertex(indexPos).getX(); // set vertex x y z positions
		vertexY = getVertex(indexPos).getY();
		vertexZ = getVertex(indexPos).getZ();

		colorR = getVertColor(indexPos).getX(); // set vertex r g b color
		colorG = getVertColor(indexPos).getY();
		colorB = getVertColor(indexPos).getZ();

		normX = getVertNorm(indexNorm).getX(); // set vertex normal x y z positions
		normY = getVertNorm(indexNorm).getY();
		normZ = getVertNorm(indexNorm).getZ();

		glNormal3f(normX, normY, normZ);
		glVertex3f(vertexX, vertexY, vertexZ);
		glColor3f(colorR, colorG, colorB);


		// THIRD vertex
		// get required indices
		indexPos = (getFacePos(i).getZ()) - 1; // faces start at 1 but first index should be 0
		indexNorm = (getFaceNorm(i).getZ()) - 1; // same as above

		vertexX = getVertex(indexPos).getX(); // set vertex x y z positions
		vertexY = getVertex(indexPos).getY();
		vertexZ = getVertex(indexPos).getZ();

		colorR = getVertColor(indexPos).getX(); // set vertex r g b color
		colorG = getVertColor(indexPos).getY();
		colorB = getVertColor(indexPos).getZ();

		normX = getVertNorm(indexNorm).getX(); // set vertex normal x y z positions
		normY = getVertNorm(indexNorm).getY();
		normZ = getVertNorm(indexNorm).getZ();

		glNormal3f(normX, normY, normZ);
		glVertex3f(vertexX, vertexY, vertexZ);
		glColor3f(colorR, colorG, colorB);


	}

	glEnd();
}

Matrix4& ParserObj::getMatrix()
{
	return model2world;
}

void ParserObj::setMatrix(Matrix4 newMatrix)
{
	model2world = newMatrix;
}

void ParserObj::parse(const char * name)
{
	FILE * pFile;
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	float v1, v2, v3;
	float n1, n2, n3;
	char c1, c2;    // characters read from file
	char ignore [100]; // ignore line
	int retValue; // number of items fscanf matched
	bool readFile = true;

	// open the file
	fopen_s(&pFile, name, "rb");
	if (pFile == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	// parse entire obj file
	while (readFile)
	{
		c1 = fgetc(pFile);
		c2 = fgetc(pFile);
		//std::cout << (i+1) << ". c1=" << c1 << " c2=" << c2 << std::endl;

		// vertex position and color
		if ((c1 == 'v') && (c2 == ' '))
		{
			retValue = fscanf_s(pFile, "%f %f %f %f %f %f ", &x, &y, &z, &r, &g, &b);
			// if it has color values set
			if (retValue == 6)
			{
				vertexPositions.push_back(Vector3(x, y, z));
				vertexColors.push_back(Vector3(r, g, b));

				//std::cout << "VAL_COL: " << x << " " << y << " " << z << " " << r << " " << g << " " << b << std::endl;
			}

			// if there are no color values
			else if (retValue == 3)
			{
				vertexPositions.push_back(Vector3(x, y, z));
				vertexColors.push_back(Vector3(1, 1, 1));

				//std::cout << "VAL: " << x << " " << y << " " << z << std::endl;
			}

		}

		// vertex normals
		else if ((c1 == 'v') && (c2 == 'n'))
		{
			retValue = fscanf_s(pFile, "%f %f %f ", &x, &y, &z);
			vertexNormals.push_back(Vector3(x, y, z));

			//std::cout << "NORMAL: " << x << " " << y << " " << z << std::endl;
		}

		// vertex faces
		else if ((c1 == 'f') && (c2 == ' '))
		{
			retValue = fscanf_s(pFile, "%f//%f %f//%f %f//%f ", &v1, &n1, &v2, &n2, &v3, &n3);
			facePositions.push_back(Vector3(v1, v2, v3));
			faceNormals.push_back(Vector3(n1, n2, n3));
			//std::cout << "FACE: " << v1 << "//" << n1 << " " << v2 << "//" << n2 << " " << 
			//			v3 << "//" << n3 << std::endl;
		}

		// ignore all other lines
		else if (c1 == '#')
		{
			fgets(ignore, 100, pFile);
			if (strstr(ignore, "End of File") != nullptr || (int(c1) == -1))
			{
				std::cout << "reached the end of the file" << std::endl;
				readFile = false;
			}
		}

		else
		{
			// do nothing so it'll sample again
			// check if there's nothing left
			if (int(c1) == -1)
			{
				std::cout << "reached the end of the file" << std::endl;
				readFile = false;
			}
		}

	}
	// remember once you draw your bunny that the file index for faces starts at 1 so
	// if you see vertex index = 1 that actually means vertex[0]!
}

/* get vertex position at a certain index */
Vector3 ParserObj::getVertex(int index)
{
	return vertexPositions[index];
}

/* get vertex color at a certain index */
Vector3 ParserObj::getVertColor(int index)
{
	return vertexColors[index];
}

/* get vertex normal at a certain index */
Vector3 ParserObj::getVertNorm(int index)
{
	return vertexNormals[index];
}

/* get vertex face at a certain index  */
Vector3 ParserObj::getFacePos(int index)
{
	return facePositions[index];
}

Vector3 ParserObj::getFaceNorm(int index)
{
	return faceNormals[index];
}

int ParserObj::getFaceSize()
{
	return facePositions.size();
}

Vector3 ParserObj::calculateAverage()
{
	int pointSize = vertexPositions.size();
	Vector3 pos = Vector3();

	for (int i = 0; i < pointSize; i++)
	{
		pos = vertexPositions[i];

		// get min and max X
		if (pos.getX() > maxX)
		{
			maxX = pos.getX();
		}
		if (pos.getX() < minX)
		{
			minX = pos.getX();
		}

		// get min and max Y
		if (pos.getY() > maxY)
		{
			maxY = pos.getY();
		}
		if (pos.getY() < minY)
		{
			minY = pos.getY();
		}

		// get min and max Z
		if (pos.getZ() > maxZ)
		{
			maxZ = pos.getZ();
		}
		if (pos.getZ() < minZ)
		{
			minZ = pos.getZ();
		}
	}

	pos = Vector3((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);

	std::cout << " minX: " << minX << " maxX: " << maxX << std::endl;
	std::cout << " minY: " << minY << " maxY: " << maxY << std::endl;
	std::cout << " minZ: " << minZ << " maxZ: " << maxZ << std::endl;

	return pos;
}

/* directly apply the average to all the points */
void ParserObj::applyAverage(Vector3 averageVec)
{
	int pointSize = vertexPositions.size();
	for (int i = 0; i < pointSize; i++)
	{
		vertexPositions[i] = Vector3(vertexPositions[i].getX() - averageVec.getX(), vertexPositions[i].getY() - averageVec.getY(),
			vertexPositions[i].getZ() - averageVec.getZ());
	}

}

// Scaling methods
void ParserObj::scaleY()
{
	objScale = (40 * tan((30.0 / 180.0) * M_PI) / (maxY - minY));
}

void ParserObj::scaleX()
{
	objScale = (40 * tan((30.0 / 180.0) * M_PI) / (maxX - minX));
}

void ParserObj::setScale(double newScale)
{
	objScale = newScale;
}

void ParserObj::setScale2(double mouseScale)
{
	objScale2 = mouseScale;
}

// rotation methods
void ParserObj::incRotX(double addAngle)
{
	angleX = angleX + addAngle;
}

void ParserObj::incRotY(double addAngle)
{
	angleY = angleY + addAngle;
}

void ParserObj::incRotZ(double addAngle)
{
	angleZ = angleZ + addAngle;
}

void ParserObj::setArbitAngle(double newAngle)
{
	arbitAngle = arbitAngle + newAngle;
}

void ParserObj::setAxis(Vector3 newAxis)
{
	axis = newAxis;
}

void ParserObj::keyEvent(unsigned char key)
{
	switch (key)
	{
	case 'x':
		angleX += spinVelocity;
		break;
	case 'X':
		angleX -= spinVelocity;
		break;
	case 'y':
		angleY += spinVelocity;
		break;
	case 'Y':
		angleY -= spinVelocity;
		break;
	case 'z':
		angleZ += 1;
		break;
	case 'Z':
		angleZ -= 1;
		break;
	case 's':
		objScale -= .5;
		break;
	case 'S':
		objScale += .5;
		break;
	}
}

ParserObj::~ParserObj()
{
}
