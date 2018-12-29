/*Class Texture Manager
  General class to manage all the textures for 2D and 3D
  Prefix: CTM_

	@author Ramon Molla
	@version 2013-06
*/

#include <SITexturesResources.h>

UGKS_String CTM_TexturesFiles[CTM_MAX_TEXTURE] =
{
	"barre3d.bmp",				//CTM_3D_BAND
	"barre.bmp",				//CTM_BAND
	"bonus1.bmp",				//CTM_BONUS1
	"pixel_vert.bmp",			//CTM_BRICK	Only one green PIXEL
	"pixel_vert3d.bmp",			//CTM_BRICK3D
	"cadre_centre.bmp",			//CTM_CADRE_CENTRE
	"cadre_centre_long.bmp",	//CTM_CADRE_CENTRE_LONG
	"cadre_energy.bmp",			//CTM_CADRE_ENERGY
	"cadre_energy_contour.bmp",	//CTM_CADRE_ENERGY_CONTOUR
	"fond_earth_moon.bmp",		//CTM_EARTH_MOON_BCKG
	"equalizer.bmp",			//CTM_EQUALIZER
	"equalizer_barre.bmp",		//CTM_EQUALIZER_BAND
	"fond_galaxy1.bmp",			//CTM_GALAXY_BCKG
	"textHealthBar.bmp",		//CTM_HEALTH_BAR
	"hiscore.bmp",				//CTM_HISCORE	Text "Hiscores"
	"lives.bmp",				//CTM_LIVES	Text "Lives"
	"SILoading2.bmp",			//CTM_LOADING
	"chiffre0.bmp",				//CTM_NUMBER0
	"chiffre1.bmp",				//CTM_NUMBER1
	"chiffre2.bmp",				//CTM_NUMBER2
	"chiffre3.bmp",				//CTM_NUMBER3
	"chiffre4.bmp",				//CTM_NUMBER4
	"chiffre5.bmp",				//CTM_NUMBER5
	"chiffre6.bmp",				//CTM_NUMBER6
	"chiffre7.bmp",				//CTM_NUMBER7
	"chiffre8.bmp",				//CTM_NUMBER8
	"chiffre9.bmp",				//CTM_NUMBER9
	"particle.bmp",				//CTM_PARTICLE
	"particle2.bmp",			//CTM_PARTICLE2	EXPLOSION
	"particle3.bmp",			//CTM_PARTICLE3
	"particle3_3d.bmp",			//CTM_PARTICLE3_3D
	"particle4.bmp",			//CTM_PARTICLE4
	"Paused.bmp",				//CTM_PAUSED	Text "Paused"
	"fond_planete.bmp",			//CTM_PLANET_BCKG	Planet background
	"player3d.bmp",				//CTM_PLAYER3D	Texture to indicate lives pending
	"PlayerShoot.bmp",			//CTM_PLAYER_SHOOT
	"PlayerShoot3D.bmp",		//CTM_PLAYER_SHOOT3D
	"PlayerShoot3DDarkBckgrd.bmp",	//CTM_PLAYER_SHOOT3D_DARK_BCKG,
	"powerup.bmp",				//CTM_POWERUP
	"PressKey.bmp",				//CTM_PRESSKEY	Text "Press any key to start"
	"score.bmp",				//CTM_SCORE	Text "Score"
	"ShipShoot.bmp",			//CTM_SHIP_SHOOT
	"ship1.bmp",				//CTM_SHIP1_1	First photogram of 2D game mode animation for the ship type 1
	"ship2.bmp",				//CTM_SHIP2_1	The same for the second type of ship
	"ship3.bmp",				//CTM_SHIP3_1	The same for the third  type of ship
	"ship1_b.bmp",				//CTM_SHIP1_2	second photogram of 2D game mode animation for the ship type 1
	"ship2_b.bmp",				//CTM_SHIP2_2	The same for the second type of ship
	"ship3_b.bmp",				//CTM_SHIP3_2	The same for the third  type of ship
	"PlayerShootDouble.bmp",	//CTM_SHOOT_3D_BASE_DOUBLE
	"SkyBoxBckg.bmp",			//CTM_SKYBOX_BCKG	Sky box Space background
	"SpaceBckg.bmp",			//CTM_SPACE_BCKG	Space background
	"sphere.bmp",				//CTM_SPHERE
	"supplyship.bmp",			//CTM_SUPPLYSHIP
	"SupplyShipShoot.bmp",		//CTM_SUPPLYSHIP_SHOOT
	"text_weapon.bmp",			//CTM_TEXT_WEAPON
	"text_bonus.bmp",			//CTM_TEXT_BONUS
};
