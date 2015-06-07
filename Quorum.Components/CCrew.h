/**
 * @description 
 *   Crew member of the ship.  This is the only real unit that is controllable.
 *   This object implements the IBasicUnit interface to allow basic movement via
 *   the CBasicUnitController if needed.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CCrew : public CGameUnit, public IBasicUnit, public IControllableUnit
{
	private:
		#pragma region Static Variables
		static int g_iReferences;
		static CMesh* g_pMesh;
		static CEffect*	g_pEffect;
		static CTexture2D* g_pTexture;
		static ICameraService* g_pCamera;
		#pragma endregion

		#pragma region Variables
		D3DXVECTOR2* m_pPosition;
		float m_fRotation;

		D3DXMATRIX* m_pRotation;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pWorld;

		bool m_bBound;

		int m_iStation;

		CShip* m_pShip;
		#pragma endregion

		#pragma region static Methods
		/**
		 * Builds the static mesh for the Ship class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice*);
		#pragma endregion

	public:
		#pragma region Static Variables
		static const float InputMoveRate;
		static const float InputRotateRate;
		#pragma endregion

		#pragma region Properties

		/**
		 * Returns the ship that the crew member is bound to
		 */
		CShip* Get_Ship();

		
		//Return the station that the crew member is bound to
		CStation* Get_BoundStation();

		//Return the station that the crew member is bound to
		int Get_BoundStationIndex();

		//Return the station that the crew member is bound to
		void Set_BoundStationIndex(CStation*);

		//Return the station that the crew member is bound to
		void Set_BoundStationIndex(int );

		//Get whether or not the crew member is bound to a station
		bool Get_IsStationBound();

		/**
		 * Returns the crew member's world transform
		 */
		D3DXMATRIX* Get_Transform();
		
		/**
		 * Sets the crew member's world transform
		 *
		 * @param	value	world transform matrix being passed in
		 */
		void Set_Transform(D3DXMATRIX* value);
		#pragma endregion

		#pragma region IBasicUnit Properties

		/**
		 * Derived from IBasicUnit
		 * Returns the move rate of the crew
		 */
		virtual float Get_MoveRate();

		/**
		 * Derived from IBasicUnit
		 * Returns the rotation rate of the crew
		 */
		virtual float Get_RotateRate();

		/**
		 * Derived from IBasicUnit
		 * returns the position of the ship
		 */
		virtual D3DXVECTOR2 Get_Position();

		/**
		 * Derived from IBasicUnit
		 * Sets the position of the ship
		 *
		 * @param	position	the position that the ship is being set to
		 */
		virtual void Set_Position(D3DXVECTOR2 position);

		virtual float Get_Rotation();

		virtual void Set_Rotation(float);
		#pragma endregion

		#pragma region IControllableUnit Properties

		/**
		 * Derived from IControllableUnit
		 * Returns a boolean checking whether the ship is bound to a controller or not
		 */
		virtual bool Get_IsBound();

		/**
		 * Derived from IControllableUnit
		 * Sets the boolean flag for if the ship object is bound to a controller or not
		 * 
		 * @param	isBound	flag to set the ship to
		 */
		virtual void Set_IsBound(bool);
		#pragma endregion

		#pragma region Constructors and Deconstructors
		/**
		 * Default constructor
		 *
		 * @param	game	Current game object
		 * @param	ship	ship that the crew member is being linked to
		 */
		CCrew(CGame* game, CShip* ship, int identifier);
		
		/**
		 * Crew member deconstructor
		 */
		~CCrew();
		#pragma endregion

		#pragma region IBasicUnit Methods
		
		/**
		 * Derived from IBasicUnit
		 * Moves the crew based on an offset
		 *
		 * @param	offset	offset passed in to add to the crew's current position
		 */
		virtual void Move(D3DXVECTOR2 offset);

		/**
		 * Derived from IBasicUnit
		 * Rotates the unit based on an angle offset
		 *
		 * @param	angle	angle to offset the rotation by
		 */
		virtual void Rotate(float angle);

		/**
		 * Derived from IBasicUnit
		 * Fire at the given target
		 *
		 * @param	target	target to fire at
		 */
		virtual void Fire(D3DXVECTOR2);
		#pragma endregion

		#pragma region ICollidableUnit Methods

		/**
		 * Derived from ICollidableUnit
		 * Checks to see if a collision has occured with another object
		 *
		 * @param	unit	the collidable unit the be checked against
		 */
		virtual bool CollisionOccured(ICollidableUnit*);
		#pragma endregion

		#pragma region Overriden Methods

		/**
		 * Derived Initialize method from the CGameComponent.
		 */
		virtual void Initialize();

		/**
		 * Derived LoadContent method from the CGameComponent.
		 */
		virtual void LoadContent();

		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Update(CGameTime*);

		/**
		 * Derived Draw method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Draw(CGameTime*);
		#pragma endregion
};