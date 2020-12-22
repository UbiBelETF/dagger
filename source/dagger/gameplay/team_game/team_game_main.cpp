	auto& ctrl = Engine::Registry().get<EnemyDescription>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& det = Engine::Registry().get<Detection>(state_.entity);
	auto& t = Engine::Registry().get<Transform>(state_.entity);

	auto& heroTransform = Engine::Registry().get<Transform>(det.who);
	auto& heroDetection = Engine::Registry().get<Detection>(det.who);
	auto& hero = Engine::Registry().get<CharacterController>(det.who);

	Vector2 detectionSides = det.GetDetectionSides(t.position, heroDetection, heroTransform.position);
	Vector3 lastSeen;
	Float32 dt = Engine::DeltaTime();


	int counter = 0;

	lastSeen = det.where;
	do
	{
		lastSeen = det.where;
		if (det.where.x > t.position.x) t.position.x += ((ctrl.speed - 2) * dt);
		if (det.where.x < t.position.x) t.position.x -= ((ctrl.speed - 2) * dt);
		if (det.where.y > t.position.y) t.position.y += ((ctrl.speed - 2) * dt);
		if (det.where.y < t.position.y) t.position.y -= ((ctrl.speed - 2) * dt);
		if ((lastSeen.x - t.position.x) >= 0 && (lastSeen.x - t.position.x) < 1 && (lastSeen.y - t.position.y) >= 0 && (lastSeen.y - t.position.y) < 1 || hero.shape==ctrl.shape) break;
		//std::cout << counter << std::endl;
		counter++;
	//	Logger::critical("infinite loop");
	} while (det.IsDetected(t.position, heroDetection, heroTransform.position));
	if (hero.shape == ctrl.shape) GoTo(EEnemyState::Patrolling, state_);

	do
	{
		if (lastSeen.x > t.position.x) t.position.x += ((ctrl.speed - 2) * dt);
		if (lastSeen.x < t.position.x) t.position.x -= ((ctrl.speed - 2) * dt);
		if (lastSeen.y > t.position.y) t.position.y += ((ctrl.speed - 2) * dt);
		if (lastSeen.y < t.position.y) t.position.y -= ((ctrl.speed - 2) * dt);

		//std::cout << counter << std::endl;
		counter++;
		//	Logger::critical("infinite loop");
	} while ((lastSeen.x - t.position.x) >= 0 && (lastSeen.x - t.position.x) < 1 && (lastSeen.y - t.position.y) >= 0 && (lastSeen.y - t.position.y) < 1);

	det.detected = false;

	//for (int i = 0; i < 10000; i++);
	//GoTo(EEnemyState::Patrolling, state_);