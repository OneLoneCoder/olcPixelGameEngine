/*
	OneLoneCoder - DataFile v1.00
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	An "easy to use" serialisation/deserialisation class that yields
	human readable hierachical files.

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2022 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021, 2022

*/

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <stack>
#include <sstream>

namespace olc::utils
{
	class datafile
	{
	public:
		inline datafile() = default;

	public:
		// Sets the String Value of a Property (for a given index)
		inline void SetString(const std::string& sString, const size_t nItem = 0)
		{
			if (nItem >= m_vContent.size())
				m_vContent.resize(nItem + 1);

			m_vContent[nItem] = sString;
		}

		// Retrieves the String Value of a Property (for a given index) or ""
		inline const std::string GetString(const size_t nItem = 0) const
		{
			if (nItem >= m_vContent.size())
				return "";
			else
				return m_vContent[nItem];
		}

		// Retrieves the Real Value of a Property (for a given index) or 0.0
		inline const double GetReal(const size_t nItem = 0) const
		{
			return std::atof(GetString(nItem).c_str());
		}

		// Sets the Real Value of a Property (for a given index)
		inline void SetReal(const double d, const size_t nItem = 0)
		{
			SetString(std::to_string(d), nItem);
		}

		// Retrieves the Integer Value of a Property (for a given index) or 0
		inline const int32_t GetInt(const size_t nItem = 0) const
		{
			return std::atoi(GetString(nItem).c_str());
		}

		// Sets the Integer Value of a Property (for a given index)
		inline void SetInt(const int32_t n, const size_t nItem = 0)
		{
			SetString(std::to_string(n), nItem);
		}

		// Returns the number of Values a property consists of
		inline size_t GetValueCount() const
		{
			return m_vContent.size();
		}

		// Checks if a property exists - useful to avoid creating properties
		// via reading them, though non-essential
		inline bool HasProperty(const std::string& sName) const
		{
			return m_mapObjects.count(sName) > 0;
		}

		// Access a datafile via a convenient name - "root.node.something.property"
		inline datafile& GetProperty(const std::string& name)
		{
			size_t x = name.find_first_of('.');
			if (x != std::string::npos)
			{
				std::string sProperty = name.substr(0, x);
				if (HasProperty(sProperty))
					return operator[](sProperty).GetProperty(name.substr(x + 1, name.size()));
				else
					return operator[](sProperty);
			}
			else
			{
				return operator[](name);
			}
		}

		// Access a numbered element - "node[23]", or "root[56].node"
		inline datafile& GetIndexedProperty(const std::string& name, const size_t nIndex)
		{
			return GetProperty(name + "[" + std::to_string(nIndex) + "]");
		}

	public:
		// Writes a "datafile" node (and all of its child nodes and properties) recursively
		// to a file.
		inline static bool Write(const datafile& n, const std::string& sFileName, const std::string& sIndent = "\t", const char sListSep = ',')
		{
			// Cache indentation level
			size_t nIndentCount = 0;
			// Cache sperator string for convenience
			std::string sSeperator = std::string(1, sListSep) + " ";

			// Fully specified lambda, because this lambda is recursive!
			std::function<void(const datafile&, std::ofstream&)> write = [&](const datafile& n, std::ofstream& file)
			{
				// Lambda creates string given indentation preferences
				auto indent = [&](const std::string& sString, const size_t nCount)
				{
					std::string sOut;
					for (size_t n = 0; n < nCount; n++) sOut += sString;
					return sOut;
				};

				// Iterate through each property of this node
				for (auto const& property : n.m_vecObjects)
				{
					// Does property contain any sub objects?
					if (property.second.m_vecObjects.empty())
					{
						// No, so it's an assigned field and should just be written. If the property
						// is flagged as comment, it has no assignment potential. First write the 
						// property name
						file << indent(sIndent, nIndentCount) << property.first << (property.second.m_bIsComment ? "" : " = ");

						// Second, write the property value (or values, seperated by provided
						// separation charater
						size_t nItems = property.second.GetValueCount();
						for (size_t i = 0; i < property.second.GetValueCount(); i++)
						{
							// If the Value being written, in string form, contains the separation
							// character, then the value must be written inside quotation marks. Note, 
							// that if the Value is the last of a list of Values for a property, it is
							// not suffixed with the separator
							size_t x = property.second.GetString(i).find_first_of(sListSep);
							if (x != std::string::npos)
							{
								// Value contains separator, so wrap in quotes
								file << "\"" << property.second.GetString(i) << "\"" << ((nItems > 1) ? sSeperator : "");
							}
							else
							{
								// Value does not contain separator, so just write out
								file << property.second.GetString(i) << ((nItems > 1) ? sSeperator : "");
							}
							nItems--;
						}

						// Property written, move to next line
						file << "\n";
					}
					else
					{
						// Yes, property has properties of its own, so it's a node
						// Force a new line and write out the node's name
						file << "\n" << indent(sIndent, nIndentCount) << property.first << "\n";
						// Open braces, and update indentation
						file << indent(sIndent, nIndentCount) << "{\n";
						nIndentCount++;
						// Recursively write that node
						write(property.second, file);
						// Node written, so close braces
						file << indent(sIndent, nIndentCount) << "}\n\n";
					}
				}

				// We've finished writing out a node, regardless of state, our indentation
				// must decrease, unless we're top level
				if (nIndentCount > 0) nIndentCount--;
			};

			// Start Here! Open the file for writing
			std::ofstream file(sFileName);
			if (file.is_open())
			{
				// Write the file starting form the supplied node
				write(n, file);
				return true;
			}
			return false;
		}

		inline static bool Read(datafile& n, const std::string& sFileName, const char sListSep = ',')
		{
			// Open the file!
			std::ifstream file(sFileName);
			if (file.is_open())
			{
				// These variables are outside of the read loop, as we will
				// need to refer to previous iteration values in certain conditions
				std::string sPropName = "";
				std::string sPropValue = "";

				// The file is fundamentally structured as a stack, so we will read it
				// in a such, but note the data structure in memory is not explicitly
				// stored in a stack, but one is constructed implicitly via the nodes
				// owning other nodes (aka a tree)

				// I dont want to accidentally create copies all over the place, nor do 
				// I want to use pointer syntax, so being a bit different and stupidly
				// using std::reference_wrapper, so I can store references to datafile
				// nodes in a std::container.
				std::stack<std::reference_wrapper<datafile>> stkPath;
				stkPath.push(n);


				// Read file line by line and process
				while (!file.eof())
				{
					// Read line
					std::string line;
					std::getline(file, line);

					// This little lambda removes whitespace from
					// beginning and end of supplied string
					auto trim = [](std::string& s)
					{
						s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
						s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
					};

					trim(line);

					// If line has content
					if (!line.empty())
					{
						// Test if its a comment...
						if (line[0] == '#')
						{
							// ...it is a comment, so ignore
							datafile comment;
							comment.m_bIsComment = true;
							stkPath.top().get().m_vecObjects.push_back({ line, comment });
						}
						else
						{
							// ...it is content, so parse. Firstly, find if the line
							// contains an assignment. If it does then it's a property...
							size_t x = line.find_first_of('=');
							if (x != std::string::npos)
							{
								// ...so split up the property into a name, and its values!

								// Extract the property name, which is all characters up to
								// first assignment, trim any whitespace from ends
								sPropName = line.substr(0, x);
								trim(sPropName);

								// Extract the property value, which is all characters after
								// the first assignment operator, trim any whitespace from ends
								sPropValue = line.substr(x + 1, line.size());
								trim(sPropValue);

								// The value may be in list form: a, b, c, d, e, f etc and some of those
								// elements may exist in quotes a, b, c, "d, e", f. So we need to iterate
								// character by character and break up the value
								bool bInQuotes = false;
								std::string sToken;
								size_t nTokenCount = 0;
								for (const auto c : sPropValue)
								{
									// Is character a quote...
									if (c == '\"')
									{
										// ...yes, so toggle quote state
										bInQuotes = !bInQuotes;
									}
									else
									{
										// ...no, so proceed creating token. If we are in quote state
										// then just append characters until we exit quote state.
										if (bInQuotes)
										{
											sToken.append(1, c);
										}
										else
										{
											// Is the character our seperator? If it is
											if (c == sListSep)
											{
												// Clean up the token
												trim(sToken);
												// Add it to the vector of values for this property
												stkPath.top().get()[sPropName].SetString(sToken, nTokenCount);
												// Reset our token state
												sToken.clear();
												nTokenCount++;
											}
											else
											{
												// It isnt, so just append to token
												sToken.append(1, c);
											}
										}
									}
								}

								// Any residual characters at this point just make up the final token,
								// so clean it up and add it to the vector of values
								if (!sToken.empty())
								{
									trim(sToken);
									stkPath.top().get()[sPropName].SetString(sToken, nTokenCount);
								}
							}
							else
							{
								// ...but if it doesnt, then it's something structural
								if (line[0] == '{')
								{
									// Open brace, so push this node to stack, subsequent properties
									// will belong to the new node
									stkPath.push(stkPath.top().get()[sPropName]);
								}
								else
								{
									if (line[0] == '}')
									{
										// Close brace, so this node has been defined, pop it from the
										// stack
										stkPath.pop();
									}
									else
									{
										// Line is a property with no assignment. Who knows whether this is useful,
										// but we can simply add it as a valueless property...
										sPropName = line;
										// ...actually it is useful, as valuless properties are typically
										// going to be the names of new datafile nodes on the next iteration
									}
								}
							}
						}
					}
				}

				// Close and exit!
				file.close();
				return true;
			}

			// File not found, so fail
			return false;
		}

	public:
		inline datafile& operator[](const std::string& name)
		{
			// Check if this "node"'s map already contains an object with this name...
			if (m_mapObjects.count(name) == 0)
			{
				// ...it did not! So create this object in the map. First get a vector id 
				// and link it with the name in the unordered_map
				m_mapObjects[name] = m_vecObjects.size();
				// then creating the new, blank object in the vector of objects
				m_vecObjects.push_back({ name, datafile() });
			}

			// ...it exists! so return the object, by getting its index from the map, and using that
			// index to look up a vector element.
			return m_vecObjects[m_mapObjects[name]].second;
		}

	private:
		// The "list of strings" that make up a property value
		std::vector<std::string> m_vContent;

		// Linkage to create "ordered" unordered_map. We have a vector of
		// "properties", and the index to a specific element is mapped.
		std::vector<std::pair<std::string, datafile>> m_vecObjects;
		std::unordered_map<std::string, size_t>       m_mapObjects;

	protected:
		// Used to identify if a property is a comment or not, not user facing
		bool m_bIsComment = false;
	};
}
