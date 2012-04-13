#include "Mesh.h"
#include "helper.h"

// "Default" constructor
Mesh::Mesh(const mPoint location,
		   const mPoint scale,
		   const mPoint rotation,
		   const Material m) : location(location),
							   scale(scale),
							   rotation(rotation),
							   material(m) {
	objHasVertexTexCoords = false;
	objHasVertexNormals = false;
}

// constructor that simply sets the material, leaving default
// parameters for location, scale, and rotation
Mesh::Mesh(const Material m) {
	objHasVertexTexCoords = false;
	objHasVertexNormals = false;
	material = m;
}

// Set material as current and draw
void Mesh::draw() {
	material.setAsCurrentMaterial();
	
	glPushMatrix();
		// Move to location
		glTranslatef(location.x, location.y, location.z);
		// Scale
		glScalef(scale.x, scale.y, scale.z);
		
		// Perform rotations
		glRotatef(rotation.x, 1.0, 0.0, 0.0);
		glRotatef(rotation.y, 0.0, 1.0, 0.0);
		glRotatef(rotation.z, 0.0, 0.0, 1.0);
		
		// Draw the mesh
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glPushMatrix();
			glCallList(objectDisplayList);
		glPopMatrix();
	glPopMatrix();
}

// Getters and setters

void Mesh::setLocation(const mPoint p) {
	location = p;
}

void Mesh::setScale(const mPoint p) {
	scale = p;
}

void Mesh::setRotation(const mPoint p) {
	rotation = p;
}

mPoint Mesh::getLocation() const {
	return location;
}

mPoint Mesh::getScale() const {
	return scale;
}

mPoint Mesh::getRotation() const {
	return rotation;
}

Material Mesh::getMaterial() const {
	return material;
}

void Mesh::setMaterial(const Material m) {
	material = m;
}

//
//  void loadOBJ(string filename, bool makeDisplayList = true)
//
//      This helper function loads a set of vertices, normals, and triangles
//  from the specified file. Note that this is terrible design; it reads into
//  global variables, which precludes us from using this function to load more
//  than one model in our whole scene... an object-oriented approach would be best.
//	makeDisplayList is usually true -- it is here so that PosableMesh can assemble
//  its displayList from the two Meshes it holds
//
void Mesh::loadOBJ(const string &filename, bool makeDisplayList)
{
    string line;

    ifstream in(filename.c_str());
    while(getline(in, line))
    {
        vector<string> tokens = tokenizeString(line, " /");
        if(tokens.size() < 3) continue;

        //the line should have a single character that lets us know if it's a...
        if(!tokens[0].compare("v"))                              //vertex
        {
            vertices.push_back(atof(tokens[1].c_str()));
            vertices.push_back(atof(tokens[2].c_str()));
            vertices.push_back(atof(tokens[3].c_str()));
        } else if(!tokens.at(0).compare("vn")) {                    //vertex normal
            vertexNormals.push_back(atof(tokens[1].c_str()));
            vertexNormals.push_back(atof(tokens[2].c_str()));
            vertexNormals.push_back(atof(tokens[3].c_str()));
        } else if(!tokens.at(0).compare("vt")) {                    //vertex tex coord
            vertexTexCoords.push_back(atof(tokens[1].c_str()));
            vertexTexCoords.push_back(atof(tokens[2].c_str()));
        } else if(!tokens.at(0).compare("f")) {                     //face!
            //now, faces can be either quads or triangles (or maybe more?)
            //split the string on spaces to get the number of verts+attrs.
            vector<string> faceTokens = tokenizeString(line, " ");
            if(faceTokens.size() != 4 && faceTokens.size() != 5)
            {
                fprintf(stderr, "ERROR: OBJ file contains an %u-sided polygon (unsupported). Skipping.\n", (unsigned int)faceTokens.size());
                continue;
            }

            //some local variables to hold the vertex+attribute indices we read in.
            //we do it this way because we'll have to split quads into triangles ourselves.
            int v[4], vn[4], vt[4];

            for(unsigned int i = 1; i < faceTokens.size(); i++)
            {
                //need to use both the tokens and number of slashes to determine what info is there.
                vector<string> groupTokens = tokenizeString(faceTokens[i], "/");
                int numSlashes = 0;
                for(unsigned int j = 0; j < faceTokens[i].length(); j++) { if(faceTokens[i][j] == '/') numSlashes++; }

                //regardles, we always get a vertex index.
                v[i-1] = atoi(groupTokens[0].c_str());

                //based on combination of number of tokens and slashes, we can determine what we have.
                if(groupTokens.size() == 2 && numSlashes == 1) 
                {
                    vt[i-1] = atoi(groupTokens[1].c_str()); objHasVertexTexCoords = true;
                } else if(groupTokens.size() == 2 && numSlashes == 2) {
                    vn[i-1] = atoi(groupTokens[1].c_str()); objHasVertexNormals = true;
                } else if(groupTokens.size() == 3) {
                    vt[i-1] = atoi(groupTokens[1].c_str()); objHasVertexTexCoords = true;
                    vn[i-1] = atoi(groupTokens[2].c_str()); objHasVertexNormals = true;
                } else if(groupTokens.size() != 1) {
                    fprintf(stderr, "Error. Malformed OBJ file, %s.\n", filename.c_str());
                    exit(1);
                }
            }    

            //now the local variables have been filled up; push them onto our global 
            //variables appropriately.
            if(faceTokens.size() == 4)
            {
                //it was a triangle; just push whichever three we have back.
                for(int i = 0; i < 3; i++)
                {
                    vertexIndices.push_back(v[i]);
                    if(objHasVertexNormals) vertexNormalIndices.push_back(vn[i]);
                    if(objHasVertexTexCoords) vertexTexCoordIndices.push_back(vt[i]);
                }
            } else if(faceTokens.size() == 5) {
                //it was a quad; we need to break it into two triangles. assuming mpoints
                //were specified CCW, the triangles are 0-1-2 and 0-2-3.

                vertexIndices.push_back(v[0]);
                vertexIndices.push_back(v[1]);
                vertexIndices.push_back(v[2]);
                vertexIndices.push_back(v[0]);
                vertexIndices.push_back(v[2]);
                vertexIndices.push_back(v[3]);

                if(objHasVertexNormals)
                {
                    vertexNormalIndices.push_back(vn[0]);
                    vertexNormalIndices.push_back(vn[1]);
                    vertexNormalIndices.push_back(vn[2]);
                    vertexNormalIndices.push_back(vn[0]);
                    vertexNormalIndices.push_back(vn[2]);
                    vertexNormalIndices.push_back(vn[3]);
                }

                if(objHasVertexTexCoords)
                {
                    vertexTexCoordIndices.push_back(vt[0]);
                    vertexTexCoordIndices.push_back(vt[1]);
                    vertexTexCoordIndices.push_back(vt[2]);
                    vertexTexCoordIndices.push_back(vt[0]);
                    vertexTexCoordIndices.push_back(vt[2]);
                    vertexTexCoordIndices.push_back(vt[3]);
                }
            }

        }
    }

    printf("%s: We read %d vertices, %d vertex normals, %d vertex tex coords, and %d faces.\n",
            filename.c_str(), (int)vertices.size()/3, (int)vertexNormals.size()/3, (int)vertexTexCoords.size()/2,  (int)vertexIndices.size()/3);

    in.close();
	
    if (makeDisplayList) createOBJDisplayList(); // And make it into a display list
    //createOBJDisplayList();

}

//
//  void createOBJDisplayList()
//
//      Given a set of vertices, vertex normals, and faces, (again, stored
//  in global variables; terrible design), create a display list that renders
//  the data. All of the affected variables are global. Bad bad bad design.
//
void Mesh::createOBJDisplayList()
{

  printf(" entering createdisplaylist\n");
	objectDisplayList = glGenLists(1);
	glNewList(objectDisplayList, GL_COMPILE);
	
	//NOTE: if your obj has texture coordinates, you can comment this and bind
	//      the appropriate texture and it should work.
	if (objHasVertexTexCoords && material.hasTextureFile()) {
		int width, height;
		unsigned char *imageData;
		bool wasRGBA;
		
		if (readTGA(material.getFilename(), width, height, imageData, wasRGBA) && 
			registerOpenGLTexture(imageData, width, height, objTexHandle)) {
		
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, objTexHandle);
		}
	}
	else glDisable(GL_TEXTURE_2D);
	
	glBegin(GL_TRIANGLES);
	for(unsigned int i = 0; i < vertexIndices.size()/3; i++)
	{
		int idx1, idx2, idx3, nIdx1, nIdx2, nIdx3, tIdx1, tIdx2, tIdx3;
		
		idx1 = vertexIndices.at(i*3+0)-1;
		idx2 = vertexIndices.at(i*3+1)-1;
		idx3 = vertexIndices.at(i*3+2)-1;
		
		if(objHasVertexNormals) 
		{
			nIdx1 = vertexNormalIndices.at(i*3+0)-1;
			nIdx2 = vertexNormalIndices.at(i*3+1)-1;
			nIdx3 = vertexNormalIndices.at(i*3+2)-1;
		}
		
		if(objHasVertexTexCoords) 
		{
			tIdx1 = vertexTexCoordIndices.at(i*3+0)-1;
			tIdx2 = vertexTexCoordIndices.at(i*3+1)-1;
			tIdx3 = vertexTexCoordIndices.at(i*3+2)-1;
		}
		
		//call the vertices, with normals and/or tex coords if they were in the file.
		if(objHasVertexNormals)
			glNormal3f(vertexNormals.at(nIdx1*3+0), vertexNormals.at(nIdx1*3+1), vertexNormals.at(nIdx1*3+2));
		if(objHasVertexTexCoords)
			glTexCoord2f(vertexTexCoords.at(tIdx1*2+0), 1.0 - vertexTexCoords.at(tIdx1*2+1));
		glVertex3f(vertices.at(idx1*3+0), vertices.at(idx1*3+1), vertices.at(idx1*3+2));
		
		if(objHasVertexNormals)
			glNormal3f(vertexNormals.at(nIdx2*3+0), vertexNormals.at(nIdx2*3+1), vertexNormals.at(nIdx2*3+2));
		if(objHasVertexTexCoords)
			glTexCoord2f(vertexTexCoords.at(tIdx2*2+0), 1.0 - vertexTexCoords.at(tIdx2*2+1));
		glVertex3f(vertices.at(idx2*3+0), vertices.at(idx2*3+1), vertices.at(idx2*3+2));
		
		if(objHasVertexNormals)
			glNormal3f(vertexNormals.at(nIdx3*3+0), vertexNormals.at(nIdx3*3+1), vertexNormals.at(nIdx3*3+2));
		if(objHasVertexTexCoords)
			glTexCoord2f(vertexTexCoords.at(tIdx3*2+0), 1.0 - vertexTexCoords.at(tIdx3*2+1));
		glVertex3f(vertices.at(idx3*3+0), vertices.at(idx3*3+1), vertices.at(idx3*3+2));
	}
	glEnd();
	glEndList();
}
