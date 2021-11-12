#include "io/ExportVTU.hpp"
#include <Eigen/Core>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <memory>
#include <string>
#include "io/Export.hpp"
#include "logging/LogMacros.hpp"
#include "mesh/Data.hpp"
#include "mesh/Edge.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/SharedPointer.hpp"
#include "mesh/Triangle.hpp"
#include "mesh/Vertex.hpp"
#include "utils/Helpers.hpp"
#include "utils/MasterSlave.hpp"
#include "utils/assertion.hpp"

namespace precice {
namespace io {

std::string ExportVTU::getVTKFormat() const
{
  return "UnstructuredGrid";
}

void ExportVTU::writeMasterCells(std::ostream &out) const
{
  out << "      <PCells>\n";
  out << "         <PDataArray type=\"Int32\" Name=\"connectivity\" NumberOfComponents=\"1\"/>\n";
  out << "         <PDataArray type=\"Int32\" Name=\"offsets\"      NumberOfComponents=\"1\"/>\n";
  out << "         <PDataArray type=\"UInt8\" Name=\"types\"        NumberOfComponents=\"1\"/>\n";
  out << "      </PCells>\n";
}

void ExportVTU::exportConnectivity(
    std::ostream &   outFile,
    const mesh::Mesh &mesh) const
{
  outFile << "         <Cells>\n";
  outFile << "            <DataArray type=\"Int32\" Name=\"connectivity\" NumberOfComponents=\"1\" format=\"ascii\">\n";
  outFile << "               ";
  for (const mesh::Triangle &triangle : mesh.triangles()) {
    writeTriangle(triangle, outFile);
  }
  for (const mesh::Edge &edge : mesh.edges()) {
    writeLine(edge, outFile);
  }
  outFile << '\n';
  outFile << "            </DataArray> \n";
  outFile << "            <DataArray type=\"Int32\" Name=\"offsets\" NumberOfComponents=\"1\" format=\"ascii\">\n";
  outFile << "               ";
  for (size_t i = 1; i <= mesh.triangles().size(); i++) {
    outFile << 3 * i << "  ";
  }
  const auto triangleOffset = 3 * mesh.triangles().size();
  for (size_t i = 1; i <= mesh.edges().size(); i++) {
    outFile << 2 * i + triangleOffset << "  ";
  }
  outFile << '\n';
  outFile << "            </DataArray>\n";
  outFile << "            <DataArray type=\"UInt8\"  Name=\"types\" NumberOfComponents=\"1\" format=\"ascii\">\n";
  outFile << "               ";
  for (size_t i = 1; i <= mesh.triangles().size(); i++) {
    outFile << 5 << "  ";
  }
  for (size_t i = 1; i <= mesh.edges().size(); i++) {
    outFile << 3 << "  ";
  }
  outFile << '\n';
  outFile << "            </DataArray>\n";
  outFile << "         </Cells>\n";
}
} // namespace io
} // namespace precice
