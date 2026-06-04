from models.models import Carriage, Train


class CarriageService:

    @staticmethod
    def distribute_carriages(trains: list[Train], carriages: list[Carriage]):
        sorted_carriages = sorted(carriages, key=lambda c: c.weight, reverse=True)
        n = len(sorted_carriages)

        prefix = [0.0] * (n + 1)
        for i, c in enumerate(sorted_carriages):
            prefix[i + 1] = prefix[i] + c.weight

        def group_weight(j, i):
            return prefix[i] - prefix[j]

        INF = float("inf")
        dp = [INF] * (n + 1)
        dp[0] = 0
        best_split: list[tuple[int, int] | None] = [None] * (n + 1)

        train_index = {t.id: t for t in trains}

        memo: dict[tuple[int, frozenset], tuple[float, tuple[int, int] | None]] = {}

        def solve(i: int, used_ids: frozenset) -> float:
            if i == 0:
                return 0
            key = (i, used_ids)
            if key in memo:
                return memo[key][0]

            best = INF
            best_choice = None

            for j in range(i):
                gw = group_weight(j, i)
                for train in trains:
                    if train.id in used_ids:
                        continue
                    if train.capacity - train.used_weight < gw:
                        continue
                    sub = solve(j, used_ids | {train.id})
                    if sub + 1 < best:
                        best = sub + 1
                        best_choice = (j, train.id)

            memo[key] = (best, best_choice)
            return best

        used = frozenset()
        total = solve(n, used)

        if total == INF:
            return {}

        distributed: dict[int, tuple[Train, list[Carriage]]] = {}
        i = n
        used_ids: frozenset = frozenset()

        while i > 0:
            _, choice = memo[(i, used_ids)]
            if choice is None:
                return {}
            j, train_id = choice
            train = train_index[train_id]
            distributed[train_id] = (train, sorted_carriages[j:i])
            used_ids = used_ids | {train_id}
            i = j

        return distributed