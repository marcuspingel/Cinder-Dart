// Copyright (c) 2013, Richard Eakin and the Dart project authors.
// Use of this source code (and the Dart itself) is governed by a
// BSD-style license that can be found in the LICENSE.txt file.

#pragma once

#include "cinder/DataSource.h"
#include "cinder/Function.h"

#include "dart_api.h"

#include <memory>
#include <vector>
#include <map>

namespace cidart {

typedef std::map<std::string, Dart_Handle> DataMap;

class DartVM {
public:

	typedef std::map<std::string, Dart_NativeFunction> NativeFunctionMap;
	typedef std::function<void( const DataMap& )>	ReceiveMapCallback;

	DartVM();

	void loadScript( ci::DataSourceRef script );
	void invoke( const std::string &functionName, int argc = 0, Dart_Handle *args = nullptr );

	void setMapReceiver( const ReceiveMapCallback& callback )	{ mReceiveMapCallback = callback; }

	void addNativeFunction( const std::string dartFuncName, Dart_NativeFunction nativeFunc )	{ mNativeFunctionMap[dartFuncName] = nativeFunc; }


	static std::string getVersionString();
private:

	std::string getCinderDartScript();

	Dart_Isolate mIsolate;
	std::vector<std::string> mVMFlags;
	ci::DataSourceRef		mSnapshot;

	NativeFunctionMap mNativeFunctionMap;

	ReceiveMapCallback mReceiveMapCallback;

	// TODO: add these all as static class methods so they don't need to be friended
	friend void toCinder( Dart_NativeArguments arguments );
	friend Dart_NativeFunction resolveName( Dart_Handle handle, int argc );
	friend Dart_Isolate createIsolateCallback( const char* script_uri, const char* main, void* data, char** error );
	friend Dart_Handle libraryTagHandler( Dart_LibraryTag tag, Dart_Handle library, Dart_Handle urlHandle );
};

} // namespace cidart