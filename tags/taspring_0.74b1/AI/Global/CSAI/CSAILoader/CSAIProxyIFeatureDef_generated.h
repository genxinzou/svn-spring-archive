// *** This is a generated file; if you want to change it, please change GlobalAIInterfaces.dll, which is the reference
// 
// This file was generated by CSAIProxyGenerator, by Hugh Perkins hughperkins@gmail.com http://manageddreams.com
// 
   System::String * get_myName()
   {
      return new System::String( string( self->get_myName() ).c_str() );
   }

   System::String * get_description()
   {
      return new System::String( string( self->get_description() ).c_str() );
   }

   double get_metal()
   {
      return self->get_metal();
   }

   double get_energy()
   {
      return self->get_energy();
   }

   double get_maxHealth()
   {
      return self->get_maxHealth();
   }

   double get_radius()
   {
      return self->get_radius();
   }

   double get_mass()
   {
      return self->get_mass();
   }

   bool get_upright()
   {
      return self->get_upright();
   }

   int get_drawType()
   {
      return self->get_drawType();
   }

   bool get_destructable()
   {
      return self->get_destructable();
   }

   bool get_blocking()
   {
      return self->get_blocking();
   }

   bool get_burnable()
   {
      return self->get_burnable();
   }

   bool get_floating()
   {
      return self->get_floating();
   }

   bool get_geoThermal()
   {
      return self->get_geoThermal();
   }

   System::String * get_deathFeature()
   {
      return new System::String( string( self->get_deathFeature() ).c_str() );
   }

   int get_xsize()
   {
      return self->get_xsize();
   }

   int get_ysize()
   {
      return self->get_ysize();
   }

