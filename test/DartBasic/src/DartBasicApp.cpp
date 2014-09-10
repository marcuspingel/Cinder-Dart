#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

#if CINDER_VERSION >= 807
	#include "cinder/app/RendererGl.h"
#endif

#include "cidart/DartVM.h"
#include "cidart/DartTypes.h"
#include "cidart/DartDebug.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DartBasicApp : public AppNative {
  public:
	void setup();
	void keyDown( KeyEvent event );
	void draw();

	void receiveMap(  const cidart::DataMap& map );

	cidart::DartVMRef mDart;

	size_t		mNumCircleSegments;
	ColorA		mCircleColor;
	float		mCircleRadius, mRotationRate;
	Anim<float> mRotation;
};

void DartBasicApp::setup()
{
	CI_LOG_I( "dart runtime version: " << cidart::DartVM::getVersionString() );

	// these values will be updated from main.dart:
	mCircleRadius = 1.0f;
	mNumCircleSegments = 3;
	mCircleColor = ColorA::white();
	mRotationRate = 2.0f;
	mRotation = 0;

	mDart = cidart::DartVM::create();

	mDart->setMapReceiver( bind( &DartBasicApp::receiveMap, this, placeholders::_1 ) );
	mDart->loadScript( loadAsset( "main.dart" ) );
}

void DartBasicApp::receiveMap( const cidart::DataMap &map )
{
	CI_LOG_I( "huzzah" );
	for( auto &mp : map ) {
		CI_LOG_V( "key: " << mp.first << ", value type: " << cidart::getTypeName( mp.second ) );
	}

	auto radiusIt = map.find( "radius" );
	if( radiusIt != map.end() )
		mCircleRadius = cidart::getValue<float>( radiusIt->second );

	auto segIt = map.find( "segments" );
	if( segIt != map.end() )
		mNumCircleSegments = cidart::getValue<int>( segIt->second );

	auto colorIt = map.find( "color" );
	if( colorIt != map.end() )
		mCircleColor = cidart::getValue<ColorA>( colorIt->second );

	auto rotationRateIt = map.find( "rotationRate" );
	if( rotationRateIt != map.end() ) {
		mRotationRate = cidart::getValue<float>( rotationRateIt->second );
		timeline().apply( &mRotation, mRotation + 360.0f, mRotationRate ).loop();
	}
}

void DartBasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'r' ) {
		CI_LOG_V( "reload." );
		mDart->loadScript( loadAsset( "main.dart" ) ); // TODO: add DartVM::reload
	}
}

void DartBasicApp::draw()
{
	gl::clear();

	gl::pushMatrices();
		gl::translate( getWindowCenter() );
		gl::rotate( mRotation );
		gl::color( mCircleColor );
		gl::drawSolidCircle( vec2( 0, 0 ), mCircleRadius, mNumCircleSegments );
	gl::popMatrices();
}

CINDER_APP_NATIVE( DartBasicApp, RendererGl )
