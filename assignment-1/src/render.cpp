#include "render.h"
#include "models.h"

using namespace std;
// ------A recursive function to traverse scene graph and render each mesh----------
// Simplified version for rendering a skeleton mesh
void render(const aiNode* node, bool links)
{
	aiMatrix4x4 m = node->mTransformation;
	aiMesh* mesh;
	aiFace* face;
	float materialCol[4] = { 1, 0, 1, 1 };
	int meshIndex;

	m.Transpose();   //Convert to column-major order
	glPushMatrix();
	glMultMatrixf((float*)&m);   //Multiply by the transformation matrix for this node

	//The scene graph for a skeleton contains at most one mesh per node
	//Skeleton meshes are always triangle meshes
	

	if(node->mNumMeshes > 0 && links) {

		if (!replace_links(node->mName)) {

			meshIndex = node->mMeshes[0];          //Get the mesh indices from the current node
			mesh = scene->mMeshes[meshIndex];    //Using mesh index, get the mesh object
			glColor4fv(materialCol);   //Default material colour

			//Draw the mesh in the current node
			for (int k = 0; k < mesh->mNumFaces; k++)
			{
				face = &mesh->mFaces[k];
				glBegin(GL_TRIANGLES);
				for (int i = 0; i < face->mNumIndices; i++) {
					int vertexIndex = face->mIndices[i];
					if (mesh->HasNormals())
						glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
				}
				glEnd();
			}
		}
	} else {
		replace_links(node->mName);
	}

	// Recursively draw all children of the current node
	for (int i = 0; i < node->mNumChildren; i++)
		render(node->mChildren[i], links);
		

	glPopMatrix();
}

