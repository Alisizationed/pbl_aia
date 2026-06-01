from models.models import Carriage, Train


class CarriageService:

    @staticmethod
    def distribute_carriages(trains: list[Train], carriages: list[Carriage]):
        distributed: dict[int, tuple[Train, list[Carriage]]] = {}

        sorted_carriages = sorted(carriages, key=lambda c: c.weight, reverse=True)
        n = len(sorted_carriages)

        sorted_trains = sorted(
            trains,
            key=lambda t: t.capacity - t.used_weight,
            reverse=True
        )

        INF = float("inf")
        dp = [INF] * (n + 1)
        dp[0] = 0

        best_train: list[list[Train | None]] = [[None] * n for _ in range(n + 1)]

        for i in range(1, n + 1):
            for j in range(i):
                group_weight = sum(c.weight for c in sorted_carriages[j:i])

                chosen = None
                best_remaining = INF
                for train in sorted_trains:
                    available = train.capacity - train.used_weight
                    if available >= group_weight and available < best_remaining:
                        chosen = train
                        best_remaining = available

                if chosen is not None and dp[j] + 1 < dp[i]:
                    dp[i] = dp[j] + 1
                    best_train[i][j] = chosen

        if dp[n] == INF:
            return distributed

        i = n
        used_train_ids = set()
        while i > 0:
            for j in range(i):
                train = best_train[i][j]
                if train is not None and dp[i] == dp[j] + 1:
                    if train.id in used_train_ids:
                        continue
                    used_train_ids.add(train.id)
                    distributed[train.id] = (train, sorted_carriages[j:i])
                    i = j
                    break

        return distributed