from numbers import Real

import numpy as np
import numpy.typing as npt


def _energy_decay(alpha: float, number_of_impulses: int) -> float:
    """
    Given the alpha value and the number of impulses, what is the target decay?
    L_db = 20 log_10 (exp(-alpha M))
    """
    return 20.0 * np.log10(np.exp(-alpha * number_of_impulses))


def _inverse_energy_decay(
        target_decay: float,
        number_of_impulses: int) -> float:
    """
    Given the energy decay and the number of impulses, what is the alpha?
    """
    return (-1 / number_of_impulses) * np.log(np.power(10, target_decay / 20))


def _make_decaying_sample(size: int, alpha: float) -> npt.NDArray[np.float32]:
    rng = np.random.default_rng()
    samples = np.zeros((size,), dtype=np.float32)
    for i in range(size):
        # * rng.uniform(0.5, 2)
        samples[i] = np.exp(-alpha * i) * rng.choice([1, -1])
    return samples


def velvet_noise(sample_rate: Real,
                 filter_length_ms: Real,
                 number_of_impulses: int,
                 target_decay: float) -> npt.NDArray[np.float32]:
    """
    Create a velvet noise discrete-time signal.

    :param sample_rate: the sample rate of the returned signal
    :param filter_length_ms: the total milliseconds of the returned signal
    :param number_of_impulses: the number of non-zero samples
        in the returned noise
    :param target_decay: the amount of decay applied on non-zero samples
    :return: a velvet noise. The returned data bit-rate is 32-bit
    """

    # Parameter value and type check
    if sample_rate <= 0:
        raise ValueError(
            'The sample rate should be a positive real number.'
            f'Got {sample_rate}')
    if filter_length_ms <= 0:
        raise ValueError(
            f'The filter length should be a positive real number.'
            f'Got {filter_length_ms}')
    if number_of_impulses <= 0:
        raise ValueError(
            'The total number of impulses cannot be non-positive.'
            f'Got {number_of_impulses}')
    if target_decay > 0:
        raise ValueError(
            f'The target decay is expected to be negative. Got {target_decay}')

    rng = np.random.default_rng()
    filter_size = np.int32(sample_rate * filter_length_ms / 1000)

    velvet_noise_return = np.zeros((filter_size,), dtype=np.float32)

    alpha = _inverse_energy_decay(target_decay, number_of_impulses)

    indices = np.sort(
        rng.choice(
            filter_size,
            size=(number_of_impulses,),
            replace=False,
            shuffle=False))
    decaying_sample = _make_decaying_sample(number_of_impulses, alpha)
    velvet_noise_return[indices] = decaying_sample

    return velvet_noise_return
