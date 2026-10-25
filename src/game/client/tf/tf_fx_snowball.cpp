//=============================================================================
//
// snowball chunks (obsolete)
//
//=============================================================================
#include "cbase.h"
#include "c_te_effect_dispatch.h"
#include "tempent.h"
#include "c_te_legacytempents.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static const char *s_pszSnowballChunkModels[] =
{
	"models/weapons/c_models/c_snowball/c_snowball_chunk01.mdl",
	"models/weapons/c_models/c_snowball/c_snowball_chunk02.mdl",
	"models/weapons/c_models/c_snowball/c_snowball_chunk03.mdl",
	"models/weapons/c_models/c_snowball/c_snowball_chunk04.mdl",
	"models/weapons/c_models/c_snowball/c_snowball_chunk05.mdl",
};

ConVar tf_snowball_chunks( "tf_snowball_chunks", "4", FCVAR_ARCHIVE );
ConVar tf_snowball_chunk_lifetime( "tf_snowball_chunk_lifetime", "4", FCVAR_ARCHIVE );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TF_SnowballBreakCallback( const CEffectData &data )
{
	int nChunks = tf_snowball_chunks.GetInt();
	if ( nChunks <= 0 )
		return;

	Vector vecNormal = data.m_vNormal;
	if ( vecNormal.IsZero() )
	{
		vecNormal = Vector( 0, 0, 1 );
	}

	for ( int i = 0; i < nChunks; i++ )
	{
		const char *pszModel = s_pszSnowballChunkModels[ RandomInt( 0, ARRAYSIZE( s_pszSnowballChunkModels ) - 1 ) ];
		model_t *pModel = (model_t *)engine->LoadModel( pszModel );
		if ( !pModel )
			continue;

		Vector vecVelocity = vecNormal * RandomFloat( 60, 140 ) + RandomVector( -120, 120 );
		vecVelocity.z += RandomFloat( 40, 120 );

		QAngle angChunk( RandomFloat( 0, 360 ), RandomFloat( 0, 360 ), RandomFloat( 0, 360 ) );
		Vector vecOrigin = data.m_vOrigin + vecNormal * 4.0f;

		C_LocalTempEntity *pTemp = tempents->SpawnTempModel( pModel, vecOrigin, angChunk, vecVelocity, tf_snowball_chunk_lifetime.GetFloat(), FTENT_NEVERDIE );
		if ( !pTemp )
			continue;

		pTemp->m_vecTempEntAngVelocity[0] = RandomFloat( -512, 511 );
		pTemp->m_vecTempEntAngVelocity[1] = RandomFloat( -255, 255 );
		pTemp->m_vecTempEntAngVelocity[2] = RandomFloat( -255, 255 );

		pTemp->m_nSkin = data.m_nColor;
		pTemp->hitSound = 0;
		pTemp->SetGravity( 1.0f );
		pTemp->flags = FTENT_FADEOUT | FTENT_GRAVITY | FTENT_COLLIDEALL | FTENT_ROTATE | FTENT_COLLISIONGROUP;
		pTemp->SetCollisionGroup( COLLISION_GROUP_DEBRIS );
	}
}

DECLARE_CLIENT_EFFECT( "TF_SnowballBreak", TF_SnowballBreakCallback );
