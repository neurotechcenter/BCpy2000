#ifndef VHT_WITNESS_PLANE_H
#define VHT_WITNESS_PLANE_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif


class vhtVector3d;
class vhtShape3D;

//: A plane for witnessing collisions.
// For internal use only.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtWitnessPlane
{
	public:
		vhtWitnessPlane(void);
		vhtWitnessPlane( vhtVector3d &offset, vhtVector3d &normal, vhtVector3d &witnes, vhtShape3D *node );
		~vhtWitnessPlane(void);

		// data get/set
		virtual void setOffset( vhtVector3d &offset ) 
		{ 
			m_offset = offset; 
		}
		
		virtual void setNormal( vhtVector3d &normal ) 
		{ 
			m_normal = normal; 
		}
		
		virtual void setWitness( vhtVector3d &witness ) 
		{ 
			m_witness = witness; 
		}
		
		virtual void setNode( vhtShape3D *node ) 
		{ 
			m_node = node; 
		}

		virtual const vhtVector3d &getOffset( void ) const 
		{ 
			return m_offset; 
		}
		
		virtual const vhtVector3d &getNormal( void ) const 
		{ 
			return m_normal; 
		}
		
		virtual const vhtVector3d &getWitness( void ) const 
		{ 
			return m_witness; 
		}
		
		virtual const vhtShape3D *getNode( void ) const 
		{ 
			return m_node; 
		}

		virtual double witnessDistance( void );
		
		virtual bool   isInside( void );

	protected:
		// test conditions
		virtual double pointDistance( vhtVector3d &point );
		virtual bool   isInside( vhtVector3d &point );

		vhtVector3d      m_offset;
		vhtVector3d      m_normal;
		vhtVector3d      m_witness;
		vhtShape3D      *m_node;
};

#endif
