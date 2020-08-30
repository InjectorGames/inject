#include <injector/graphics/mesh.hpp>
#include <stdexcept>

namespace INJECTOR_NAMESPACE
{
	Mesh::Mesh(
		size_t _indexCount,
		BufferIndex _indexType) :
		indexCount(_indexCount),
		indexType(_indexType)
	{}
	Mesh::~Mesh()
	{}

	void Mesh::setVertexData(void* data, size_t size)
	{
		throw std::runtime_error("Not implemented mesh function");
	}
	void Mesh::setVertexData(void* data, size_t size, size_t offset)
	{
		throw std::runtime_error("Not implemented mesh function");
	}

	void Mesh::setIndexData(void* data, size_t size)
	{
		throw std::runtime_error("Not implemented mesh function");
	}
	void Mesh::setIndexData(void* data, size_t size, size_t offset)
	{
		throw std::runtime_error("Not implemented mesh function");
	}
}
