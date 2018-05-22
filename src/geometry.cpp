// Project Header
#include "geometry.h"

// --------------------------------------------------------------------------------
// Create Sphere
void createSpere(vector<glm::vec4> &buffer, vector<glm::ivec3> &indexes) {
}

void createSphere(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes, float r, int sub) {
    for (int i1 = 0; i1 < sub; i1++) {
        // Theta [0, 2pi)
        float theta = i1 * M_PI * 2.0 / sub;
        
        int offset1 = ((i1 < (sub - 1)) ? 1 : -(sub - 1)) * sub;
        
        for(int i2 = 0; i2 < sub; i2++) {
            // Phi [0, 2pi)
            float phi = i2 * M_PI * 2.0 / sub;
            
            // Cauculate position
            glm::vec4 p = glm::vec4(r * cos(theta) * cos(phi), r * sin(phi), r * sin(theta) * cos(phi), 1.0f);
            
            // Compute normal
            glm::vec4 u = glm::normalize(glm::vec4(p.x, p.y, p.z, 0.0f));
            
            // Add Position and Normal to buffer
            buffer.push_back(p);
            buffer.push_back(u);
            
            // Calculate index
            int k = i1 * sub + i2;
            
            int offset2 = (i2 < (sub - 1)) ? 1 : -(sub - 1);
            
            // Added indexes
            indexes.push_back(glm::ivec3(k, k + offset2, k + offset1 + offset2));
            indexes.push_back(glm::ivec3(k, k + offset1 + offset2, k + offset1));
        }
    }
}


// --------------------------------------------------------------------------------
// Create Sphere with Positions and Normals
void createSphereData(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes, float r, int sub1, int sub2) {
	// Longitude
	for(int i1 = 0; i1 < sub1; i1++) {
		// Theta [0, pi]
		float theta = i1 * M_PI / (sub1-1);

		// Longitude offset
		int offset1 = -sub2;

		// Latitude
		for(int i2 = 0; i2 < sub2; i2++) {
			// Phi [0, 2pi)
			float phi = i2 * M_PI * 2.0 / sub2;

			// Calculate point
			glm::vec4 p = glm::vec4(r*sin(theta)*cos(phi), r*cos(theta), r*sin(theta)*sin(phi), 1.0f);

			// Calculate normal
			glm::vec4 u = glm::normalize(glm::vec4(p.x, p.y, p.z, 0.0f));

			// Add position, normal and UV to buffer
			buffer.push_back(p);
			buffer.push_back(u);
			buffer.push_back(glm::vec4(phi / (M_PI*2.0f), theta / M_PI, 0.0f, 1.0f));

			// Latitude offset
			int offset2 = (i2 < (sub2 - 1)) ? 1 : -(sub2 - 1);

			// Add triangles between layers
			if(i1 > 0) {
				// Index of current vertex
				int k = i1*sub2 + i2;

				// Add Indexes
				indexes.push_back(glm::ivec3(k + offset1, k,           k + offset2));
				indexes.push_back(glm::ivec3(k + offset1, k + offset2, k + offset1 + offset2));
			}
		}
	}
}
