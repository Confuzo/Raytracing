#ifndef _TRIANGLE_
#define _TRIANGLE_

#include <memory>
#include "shape.hpp"
#include "paramset.hpp"
#include "../lib/tiny_obj_loader.h"
#include <bits/stdc++.h>


/// This struct implements an indexd triangle mesh database.
struct TriangleMesh {
    int n_triangles; //!< # of triangles in the mesh.
    // The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
    std::vector< int > vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
    std::vector< int > normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
    std::vector< int > uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

    // Here comes the data associated with each vertex. WARNING: The length of the vectors below may differ.
    std::vector< Vec > vertices;  //!< The 3D geometric coordinates
    std::vector< Vec > normals;  //!< The 3D normals.
    std::vector< Point2 > uvcoords; //!< The 2D texture coordinates.

    // Regular constructor (almost all data is imported via move semantics).
    TriangleMesh(){/*empty*/};
    /// Copy constructor disabled
    TriangleMesh( const TriangleMesh& ) = delete;
    /// Assign operator disabled.
    TriangleMesh& operator=( const TriangleMesh & ) = delete;
    /// Move constructor.
    TriangleMesh( TriangleMesh && other ) = delete;
};


/// Represents a single triangle.
class Triangle : public Shape {
	private:
		int *v; //!< Pointer to the vertex index array stored in the triangle mesh database. 
		// After the proper triangle initialization, we may access vertices with v[0], v[1], and v[2]
		int *n; //!< Pointer to the normal index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access normals with n[0], n[1], and n[2]
		int *uv;//!< Pointer to the UV coord index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access UV coords with uv[0], uv[1], and uv[2]

		std::shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
		bool backface_cull;            //!< Set it `true` to activate backface culling.
	
	public:
        // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
		Triangle( std::shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc=true )
			: Shape(), mesh{mesh}, backface_cull{bfc}
		{
			// This is just a shortcut to access this triangle's data stored in the mesh database.
			v  = &mesh->vertex_indices[ 3 * tri_id ];
			n  = &mesh->normal_indices[ 3 * tri_id ];
			uv = &mesh->uvcoord_indices[ 3 * tri_id ];
		}
		/// Return the triangle's bounding box.
		// Bounds3f object_bound() const;
        /// The regular intersection methods, as defined in the Shape parent class.
		bool intersect( Ray& r,float * t_hit, Surfel *sf ) {return true;}
        bool intersect_p( Ray & r ) {return true;}

        void printCenter(){}

        /// This friend function helps us debug the triangles, if we want to.
        friend std::ostream& operator<<( std::ostream& os, const Triangle & t );

        std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape( bool flip_normals,  Paramset<std::string, std::string> ps );
        std::vector<std::shared_ptr<Shape>> create_triangle_mesh( std::shared_ptr<TriangleMesh> mesh , bool backface_cull);
        bool load_mesh_data( const std::string & filename, bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md );
        void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md );
};

std::ostream& operator<<( std::ostream& os, const Triangle & t )
{
    constexpr short X{0};
    constexpr short Y{1};
    constexpr short Z{2};

    os  << "    + V0: ("
        << t.mesh->vertices[ t.v[0] ].v1 << ", "
        << t.mesh->vertices[ t.v[0] ].v2 << ", "
        << t.mesh->vertices[ t.v[0] ].v3 << ")\n"
        << "    + V1: ("
        << t.mesh->vertices[ t.v[1] ].v1 << ", "
        << t.mesh->vertices[ t.v[1] ].v2 << ", "
        << t.mesh->vertices[ t.v[1] ].v3 << ")\n"
        << "    + V2: ("
        << t.mesh->vertices[ t.v[2] ].v1 << ", "
        << t.mesh->vertices[ t.v[2] ].v2 << ", "
        << t.mesh->vertices[ t.v[2] ].v3 << ")\n"

        << "    + N0: ("
        << t.mesh->normals[ t.n[0] ].v1 << ", "
        << t.mesh->normals[ t.n[0] ].v2 << ", "
        << t.mesh->normals[ t.n[0] ].v3 << ")\n"
        << "    + N1: ("
        << t.mesh->normals[ t.n[1] ].v1 << ", "
        << t.mesh->normals[ t.n[1] ].v2 << ", "
        << t.mesh->normals[ t.n[1] ].v3 << ")\n"
        << "    + N2: ("
        << t.mesh->normals[ t.n[2] ].v1 << ", "
        << t.mesh->normals[ t.n[2] ].v2 << ", "
        << t.mesh->normals[ t.n[2] ].v3 << ")\n"

        << "    + UV0: ("
        << t.mesh->uvcoords[ t.uv[0] ].u << ", "
        << t.mesh->uvcoords[ t.uv[0] ].v << ")\n"
        << "    + UV1: ("
        << t.mesh->uvcoords[ t.uv[1] ].u << ", "
        << t.mesh->uvcoords[ t.uv[1] ].v << ")\n"
        << "    + UV2: ("
        << t.mesh->uvcoords[ t.uv[2] ].u << ", "
        << t.mesh->uvcoords[ t.uv[2] ].v << ")\n";

    return os;
}

/// This is the function that actually creates the mesh database and the triangles, ans store them in a Shape list.
std::vector<std::shared_ptr<Shape>> create_triangle_mesh( std::shared_ptr<TriangleMesh> mesh , bool backface_cull){
    // List of shapes (triangles) we need to return to the client.
	std::vector<std::shared_ptr<Shape>> tris;
	// Create the triangles, which are just references to the mesh database.
	for ( int i = 0 ; i < mesh->n_triangles ; ++i )
		tris.push_back(std::make_shared<Triangle>( mesh, i, backface_cull ) );

	return tris;

}

/// Internal function that calls the tinyobjloader api to read the OBJ data into memory.
bool load_mesh_data( const std::string & filename, bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md ){}

/// This function essentially convertes the mesh data from the tinyobjloader internal data structure to our mesh data structure.
void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md ){}

const std::vector<std::string> explode(const std::string& s, const char& c)
{
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}                       

/// This is the entry point for the client. This function begins the process of reading a triangle mesh.
std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape( bool flip_normals,  Paramset<std::string, std::string> ps ){
    bool bkfc_on{true};
    bool reverse_vertex_order{false};
    bool compute_normals{false};


    std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>();
    std::string filename = ps.find("filename");
    std::string bkf_on_str = ps.find("backface_cull");
    
    std::string rvo_str = ps.find("reverse_vertex_order");
    if ( rvo_str == "on" or rvo_str == "true" )
        reverse_vertex_order = true;
    
    // Retrieve compute normals flag
    std::string cn_str = ps.find("compute_normals");
    if ( cn_str == "on" or cn_str == "true" )
        compute_normals = true;

    if ( filename != "" ) { // Read mesh data from an OBJ file provided in the scene file.
        // Call our auxiliary function that interfaces with tinyobjloader API.
        if ( load_mesh_data( filename, reverse_vertex_order, compute_normals, flip_normals, mesh ) )
            std::cout << ">>> Mesh data successfuly loaded!\n";
        else
            std::cout << ">>> Mesh data loading failed!\n";
    }
    else { // This means we must read the mesh data from the scene file, not from the OBJ file.
        int num_triangles =  std::stoi(ps.find("ntriangles"));
        std::string indices = ps.find("indices");
        //std::vector<std::string> i{explode(indices, '  ')};
        std::string vertices = ps.find("vertices");
        std::string normals = ps.find("normals");
        std::string uv = ps.find("uv");

        std::vector<std::string> i_{explode(indices, ' ')};
        std::vector<std::string> v_{explode(vertices, ' ')};
        std::vector<std::string> n_{explode(normals, ' ')};
        std::vector<std::string> uv_{explode(uv, ' ')};
        
        std::vector<Vec> ind;
        for(int i = 0; i < i_.size(); ){
           Vec v(std::stof(i_[i+0]),std::stof(i_[i+1]),std::stof(i_[i+2]));
           i+=3;
           ind.push_back(v);
        }

        std::vector<Vec> vert;
        for(int i = 0; i < v_.size(); ){
           Vec v(std::stof(v_[i+0]),std::stof(v_[i+1]),std::stof(v_[i+2]));
           i+=3;
           vert.push_back(v);
        }

        std::vector<Vec> norm;
        for(int i = 0; i < n_.size(); ){
           Vec v(std::stof(n_[i+0]),std::stof(n_[i+1]),std::stof(n_[i+2]));
           i+=3;
           norm.push_back(v);
        }

        std::vector<Point2> uvs;
        for(int i = 0; i < uv_.size(); ){
           Point2 v(std::stof(uv_[i+0]),std::stof(uv_[i+1]));
           i+=2;
           uvs.push_back(v);
        }

        mesh->n_triangles = num_triangles;
        mesh->vertices = vert;
        mesh->normals = norm;
        mesh->uvcoords = uvs;

        for(int i =0; i < ind.size(); i++){
            mesh->vertex_indices.push_back(ind[i].v1);
            mesh->vertex_indices.push_back(ind[i].v2);
            mesh->vertex_indices.push_back(ind[i].v3);

            mesh->normal_indices.push_back(ind[i].v1);
            mesh->normal_indices.push_back(ind[i].v2);
            mesh->normal_indices.push_back(ind[i].v3);

            mesh->uvcoord_indices.push_back(ind[i].v1);
            mesh->uvcoord_indices.push_back(ind[i].v2);
            mesh->uvcoord_indices.push_back(ind[i].v3);
        }

    }

    return create_triangle_mesh(mesh, bkfc_on);
}

#endif
