/**
 * @description 
 *   Basic underlying class for most of the objects in the game.  Implements bounding box drawing
 *   ,basic attribute maintainence and collision detection callback.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CGameUnit : public CDrawableGameComponent, public ICollidableUnit
{
	private:
		#pragma region Static Variables
		static ICameraService* g_pCamera;
		static IWorldScroller* g_pWorldScroller;
		#pragma endregion

		#pragma region Variables
		CBoundingPolygon* m_pBoundingPolygon;
		CBoundingBox* m_pBoundingBox;

		int m_iIdentifier;

		int m_pHp;
		int m_pMaxHP;
		int m_pAttackPower;
		int m_pDefensePower;
		int m_pAttackRadius;
		#pragma endregion

		#pragma region Methods
		/**
		 * Builds the static mesh for the GameUnit class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		CMesh* BuildBoundingMesh(CGraphicsDevice*);
		#pragma endregion

	public:

		#pragma region Properties
		int Get_HP();
		void Set_HP(int);
		
		int Get_MaxHP();
		void Set_MaxHP(int);

		int Get_AttackPower();
		void Set_AttackPower(int);

		int Get_DefensePower();
		void Set_DefensePower(int);

		int Get_AttackRadius();
		#pragma endregion

		#pragma region Constructors and Finalizers
		/**
		 * Default constructor
		 *
		 * @param	game	current game
		 */
		CGameUnit(CGame* game, int identifier);

		/**
		 * Game unit Deconstructor
		 */
		~CGameUnit();
		#pragma endregion

		#pragma region Properties
		/**
		 * Return the camera set to the game unit
		 */
		ICameraService* Get_Camera();

		/**
		 * Returns the identifier of this game object
		 */
		int Get_Identifier();

		void Set_Identifier(int);
		#pragma endregion

		#pragma region ICollidableUnit Properties
		/**
		 * Derived from ICollidableUnit
		 * Sets the bounding volume for the game unit
		 *
		 * @param	size	size of the bounding volume
		 * @param	segments	segments of the bounding volume
		 */
		virtual CBoundingBox* Get_BoundingBox();
		#pragma endregion

		#pragma region IAdvancedCollidableUnit Property
		/**
		 * Sets the bounding volume for the game unit
		 *
		 * @param	size	size of the bounding volume
		 * @param	segments	segments of the bounding volume
		 */
		CBoundingPolygon* Get_BoundingPolygon();
		#pragma endregion

		#pragma region ICollidableUnit Methods
		/**
		 * Derived from ICollidableUnit
		 * Checks to see if a collision has occured with another object
		 *
		 * @param	unit	the collidable unit the be checked against
		 */

		virtual bool CollisionOccured(ICollidableUnit*) = 0;
		#pragma endregion

		#pragma region Overriden Methods

		/**
		 * Derived Initialize method from the CDrawableGameComponent.
		 */
		virtual void Initialize();

		/**
		 * Derived LoadContent method from CDrawableGameComponent
		virtual void LoadContent();
		/**
		 * Derived Draw method from the CDrawableGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Draw(CGameTime*);
		#pragma endregion
};
