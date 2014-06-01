// (C) Copyright Gert-Jan de Vos and Jan Wilmans 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// Repository at: https://github.com/djeedjay/DebugViewPP/

#pragma once

#include "DebugView++Lib/Line.h"
#include "DebugView++Lib/SourceType.h"

//#define ENABLE_EXPERIMENTAL_CIRCULAR_BUFFER

namespace fusion {
namespace debugviewpp {

class ILineBuffer;
class LogSource
{
public:
	LogSource(SourceType::type sourceType, ILineBuffer& linebuffer);
	virtual ~LogSource();

	virtual bool AtEnd() const = 0;
	virtual HANDLE GetHandle() const = 0;
	virtual void Notify() = 0;
	virtual Lines GetLines() = 0;		// replaced by LogSources::GetLines(), remove once circular buffer is completed

	std::wstring GetDescription() const;
	void SetDescription(const std::wstring& description);
	SourceType::type GetSourceType() const;
	void Add(double time, FILETIME systemTime, HANDLE handle, const char* message);

private:
	ILineBuffer& m_linebuffer;
	std::wstring m_description;
	SourceType::type m_sourceType;
};

} // namespace debugviewpp 
} // namespace fusion
