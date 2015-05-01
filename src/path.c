#include "path.h"


struct path_s *AddPath( struct path_s *path, vec2_t point )
{
	struct path_s *temp;

	temp = ( struct path_s * )malloc( sizeof( struct path_s ) );
	Vec2Copy( point, temp->pos );
	temp->next = path;
	path = temp;

	return path;
}


struct path_s *ClearPathing( struct path_s *path )
{
	struct path_s *temp;

	while( temp )
	{
		temp = path;
		path = path->next;

		free( temp );
	}

	return NULL;
}


void NextPosition( struct entity_s *ent )
{
	struct path_s *temp;

	temp = ent->path;
	ent->path = ent->path->next;

	free( temp );
}
